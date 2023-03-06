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
#define FTM_CHANNEL_DC_MOTOR	//Define DC motor channel
#define FTM_CHANNEL_SERVO		//Define servo PWM channel

extern QueueHandle_t motor_queue, angle_queue;

void setupMotorComponent();
void setupDCMotor();
void setupServo();
void setupMotorPins();

void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle);

void motorTask(void* pvParameters);
void positionTask(void* pvParameters);


#endif /* MOTOR_CONTROL_COMPONENT_H */
