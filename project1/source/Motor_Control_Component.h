#ifndef MOTOR_CONTROL_COMPONENT_H
#define MOTOR_CONTROL_COMPONENT_H

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_ftm.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Terminal_Component.h"

#define FTM_MOTORS 				FTM0
#define FTM_CHANNEL_DC_MOTOR	kFTM_Chnl_0 //Define DC motor channel
#define FTM_CHANNEL_SERVO		kFTM_Chnl_3 //Define servo PWM channel
											//! Could be wrong
#define INIT_MODE0_SPEED 0
#define INIT_MODE1_SPEED 30
#define INIT_MODE2_SPEED 90

#define dc_speed_to_dutycycle(speed) speed * 0.025f/100.0f + 0.0615
#define direction_to_coeff(direction) direction == FORWARD ? 1 : -1

typedef enum {
	SPEED_MODE,
	SPEED_COMPENSATION
} MotorMesssage_t;

typedef enum {FORWARD, BACKWARD} SpeedDirection_t;
typedef enum {MODE0, MODE1, MODE2} SpeedMode_t;

typedef struct __MotorQueueMessage_t__ {
	MotorMesssage_t type;
	void *payload;
} MotorQueueMessage_t;

typedef struct __SpeedModePayload_t__ {
	SpeedDirection_t direction;
	SpeedMode_t mode;
} SpeedModePayload_t;

typedef struct __ModeSpeeds_t__ {
	int mode0_speed, mode1_speed, mode2_speed;
} ModeSpeeds_t;

extern QueueHandle_t motor_queue, angle_queue;

void setupMotorComponent();
void setupDCMotor();
void setupServo();
void setupMotorPins();
void setupPWM(ftm_chnl_t);

void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle);

void motorTask(void* pvParameters);
void positionTask(void* pvParameters);

void updateMotorSpeed(MotorQueueMessage_t message);


#endif /* MOTOR_CONTROL_COMPONENT_H */
