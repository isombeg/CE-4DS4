#include "Motor_Control_Component.h"

QueueHandle_t motor_queue;
QueueHandle_t angle_queue;

void setupMotorComponent()
{
	setupMotorPins();

	setupDCMotor();
	setupServo();

    /*************** Motor Task ***************/
	//Create Motor Queue and Motor Task
	setupMotorComponentTask(motorTask, &motor_queue, "motorTask");

    /*************** Position Task ***************/
	//Create Angle Queue and Position Task
	setupMotorComponentTask(positionTask, &angle_queue, "positionTask");
}

void setupMotorComponentTask(void task(void*), QueueHandle_t* queue, char* taskName){
	if ((*queue = xQueueCreate(10, sizeof(int))) == NULL){
		PRINTF("[Motor Component] %s queue creation failed!\r\n", taskName);
		while(1);
	}

	//Create Position Task
	if((xTaskCreate(task, taskName, 200, NULL, 3, NULL)) != pdPASS){
		PRINTF("[Motor Component] %s creation failed!\r\n", taskName);
		while(1);
	}
}

void setupMotorPins()
{
    //Configure PWM pins for DC and Servo motors
	return;

}

void setupDCMotor()
{
	//Initialize PWM for DC motor
	setupPWM(FTM_CHANNEL_DC_MOTOR);

	updatePWM_dutyCycle(
		FTM_CHANNEL_DC_MOTOR,
		dc_speed_to_dutycycle(0)
	);

	FTM_SetSoftwareTrigger(FTM_MOTORS, true);
	return;
}

void setupServo()
{
	//Initialize PWM for Servo motor
	setupPWM(FTM_CHANNEL_SERVO);

	FTM_SetSoftwareTrigger(FTM_MOTORS, true);
	return;
}

void setupPWM(ftm_chnl_t chnlNumber)
{
	ftm_config_t ftmInfo;
	ftm_chnl_pwm_signal_param_t ftmParam;
	ftm_pwm_level_select_t pwmLevel = kFTM_HighTrue;

	ftmParam.chnlNumber = chnlNumber;
	ftmParam.level = pwmLevel;
	ftmParam.dutyCyclePercent = 7;
	ftmParam.firstEdgeDelayPercent = 0U;
	ftmParam.enableComplementary = false;
	ftmParam.enableDeadtime = false;

	FTM_GetDefaultConfig(&ftmInfo);
	ftmInfo.prescale = kFTM_Prescale_Divide_128;

	FTM_Init(FTM_MOTORS, &ftmInfo);
	FTM_SetupPwm(FTM_MOTORS, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 50U, CLOCK_GetFreq(
	kCLOCK_BusClk));
	FTM_StartTimer(FTM_MOTORS, kFTM_SystemClock);
}

void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle)
{
	uint32_t cnv, cnvFirstEdge = 0, mod;

	/* The CHANNEL_COUNT macro returns -1 if it cannot match the FTM instance */
	assert(-1 != FSL_FEATURE_FTM_CHANNEL_COUNTn(FTM_MOTORS));

	mod = FTM_MOTORS->MOD;
	if(dutyCycle == 0U)
	{
		/* Signal stays low */
		cnv = 0;
	}
	else
	{
		cnv = mod * dutyCycle;
		/* For 100% duty cycle */
		if (cnv >= mod)
		{
			cnv = mod + 1U;
		}
	}

	FTM_MOTORS->CONTROLS[channel].CnV = cnv;
}

void motorTask(void* pvParameters)
{
	//Motor task implementation

	BaseType_t status;
	int speed;

	setupDCMotor();

	while(1){
		status = xQueueReceive(motor_queue, (void *) &speed, portMAX_DELAY);
		if(status != pdPASS){
			PRINTF("[Motor Task] Queue Receive failed!.\r\n");
			while (1);
		}

		PRINTF("[Motor Task] Received value\r\n");

		PRINTF("[Motor Task] Updating speed to %d\r\n", speed);
		updatePWM_dutyCycle(
			FTM_CHANNEL_DC_MOTOR,
			dc_speed_to_dutycycle(speed)
		);
//		FTM_SetSoftwareTrigger(FTM_MOTORS, true);
//		vTaskDelay(10/portTICK_PERIOD_MS);

	}
}


void positionTask(void* pvParameters)
{
	//Position task implementation
	BaseType_t status;
	int angle;

	setupServo();

	while(1){
		status = xQueueReceive(angle_queue, (void *) &angle, portMAX_DELAY);
		if(status != pdPASS){
			PRINTF("[Position Task] Queue Receive failed!.\r\n");
			while (1);
		}

		PRINTF("[Position Task] Received value\r\n");

		// update servo position
		PRINTF("[Position Task] Updating position to %d\r\n", angle);
		updatePWM_dutyCycle(
			FTM_CHANNEL_SERVO,
			angle_to_dutycycle(angle)
		);
		FTM_SetSoftwareTrigger(FTM_MOTORS, true);
//		vTaskDelay(10/portTICK_PERIOD_MS);

	}
}
