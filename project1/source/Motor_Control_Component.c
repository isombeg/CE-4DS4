#include "Motor_Control_Component.h"

QueueHandle_t motor_queue;
QueueHandle_t angle_queue;

ModeSpeeds_t mode_speeds;

void setupMotorComponent()
{
	setupMotorPins();

	setupDCMotor();
	setupServo();

    /*************** Motor Task ***************/
	//Create Motor Queue
	motor_queue = xQueueCreate(10, sizeof(MotorQueueMessage_t));
	if (motor_queue == NULL)
	{
		PRINTF("Queue creation failed!.\r\n");
		while (1);
	}

	//Create Motor Task
	status_t status = xTaskCreate(motorTask, "Motor Task", 200, NULL, 3, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}

    /*************** Position Task ***************/
	//Create Angle Queue
	//Create Position Task
}

void setupMotorPins()
{
    //Configure PWM pins for DC and Servo motors
	return;

}

void setupDCMotor()
{
	//Initialize PWM for DC motor
	setupPWM(FTM_CHANNEL_SERVO);
	updatePWM_dutyCycle(
		FTM_CHANNEL_DC_MOTOR,
		dc_speed_to_dutycycle(0)
	);
	return;
}

void setupServo()
{
	//Initialize PWM for Servo motor
	setupPWM(FTM_CHANNEL_DC_MOTOR);
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
	MotorQueueMessage_t message;

	setupDCMotor();

	while(1){
		status = xQueueReceive(motor_queue, (void *) &message, portMAX_DELAY);
		if(status != pdPASS){
			PRINTF("[Motor Task] Queue Receive failed!.\r\n");
			while (1);
		}

		PRINTF("[Motor Task] Received value\r\n");

		updateMotorSpeed(message);
		vTaskDelay(10/portTICK_PERIOD_MS);

	}
}

void updateMotorSpeed(MotorQueueMessage_t message){
	if(message.type == SPEED_COMPENSATION){
		//Compensate
		//Update PWM
	} else {
		int speed;
		SpeedModePayload_t payload = *((SpeedModePayload_t *) message.payload);

		switch(payload.mode){
		case MODE0:
			speed = INIT_MODE0_SPEED;
			break;
		case MODE1:
			speed = INIT_MODE1_SPEED;
			break;
		case MODE2:
			speed = INIT_MODE2_SPEED;
			break;
		}

		updatePWM_dutyCycle(
			FTM_CHANNEL_DC_MOTOR,
			direction_to_coeff(payload.direction) * dc_speed_to_dutycycle(speed)
		);
	}
}



void positionTask(void* pvParameters)
{
	//Position task implementation

}


