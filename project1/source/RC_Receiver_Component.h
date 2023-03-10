#ifndef RC_RECEIVER_COMPONENT_H
#define RC_RECEIVER_COMPONENT_H

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_uart.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "Motor_Control_Component.h"
#include "LED_Component.h"

#define RC_UART		//Define UART for RC receiver
#define MODE0_SPEED_SCALE .10
#define MODE1_SPEED_SCALE .25
#define MODE2_SPEED_SCALE 1

#define rc_to_angle(rc_values) (45*(rc_values.ch1-1500))/500
#define rc_to_speed(rv_values, direction, scale) scale*direction * (100*(rc_values.ch3-1000))/1000;

extern SemaphoreHandle_t rc_hold_semaphore;
extern TaskHandle_t rc_task_handle;

void setupRCReceiverComponent();
void setupUART_RC();
void setupRCPins();

void rcTask(void* pvParameters);

#endif /* RC_RECEIVER_COMPONENT_H */
