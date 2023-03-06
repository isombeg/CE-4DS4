#include "Motor_Control_Component.h"

QueueHandle_t motor_queue;
QueueHandle_t angle_queue;

void setupMotorComponent()
{
	setupMotorPins();

	setupDCMotor();
	setupServo();

    /*************** Motor Task ***************/
	//Create Motor Queue
	//Create Motor Task

    /*************** Position Task ***************/
	//Create Angle Queue
	//Create Position Task
}

void setupMotorPins()
{
    //Configure PWM pins for DC and Servo motors
}

void setupDCMotor()
{
	//Initialize PWM for DC motor
}

void setupServo()
{
	//Initialize PWM for Servo motor
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
}

void positionTask(void* pvParameters)
{
	//Position task implementation
}
