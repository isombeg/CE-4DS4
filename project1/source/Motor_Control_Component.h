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

#define DC_MOTOR_PWM_OFFSET 0.0615
#define DC_MOTOR_SPEED_SCALE 100.0f
#define DC_MOTOR_PWM_HALF_SCALE 0.025f
//DC_MOTOR_PWM_HALF_SCALE *
//dc_dutyCycle =  dc_speed* 0.00025f + 0.0615;
//servo_dutyCycle = servo_angle * 0.00025f + 0.075;
#define dc_speed_to_dutycycle(speed) .00025f*speed + DC_MOTOR_PWM_OFFSET

#define POS_MOTOR_PWM_OFFSET 0.068
#define POS_MOTOR_ANGLE_SCALE 45.0f
#define POS_MOTOR_PWM_HALF_SCALE 0.025f
#define angle_to_dutycycle(angle) angle * POS_MOTOR_PWM_HALF_SCALE/POS_MOTOR_ANGLE_SCALE + POS_MOTOR_PWM_OFFSET

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

#endif /* MOTOR_CONTROL_COMPONENT_H */
