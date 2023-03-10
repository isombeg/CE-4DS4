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

#define dc_speed_to_dutycycle(speed) 0.025f * speed /100.0f + 0.0615
#define angle_to_dutycycle(angle) angle * 0.025f/45.0f + 0.075

extern QueueHandle_t motor_queue, angle_queue;

void setupMotorComponent();
void setupMotorComponentTask(void (void*), QueueHandle_t*, char*);
void setupDCMotor();
void setupServo();
void setupMotorPins();
void setupPWM(ftm_chnl_t);

void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle);

void motorTask(void* pvParameters);
void positionTask(void* pvParameters);

void updateMotorSpeed(MotorQueueMessage_t message);

#endif /* MOTOR_CONTROL_COMPONENT_H */
