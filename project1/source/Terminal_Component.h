#ifndef TERMINAL_COMPONENT_H
#define TERMINAL_COMPONENT_H

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_uart.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "timers.h"
#include "semphr.h"

#include "Motor_Control_Component.h"
#include "RC_Receiver_Component.h"
#include "LED_Component.h"

#include <stdarg.h>

#define TARGET_UART				UART4

#define LEFT_BIT				//Define event group bit
#define RIGHT_BIT				//Define event group bit
#define UP_BIT					//Define event group bit
#define DOWN_BIT				//Define event group bit

extern EventGroupHandle_t event_group;
extern QueueHandle_t uart_queue;

void setupTerminalComponent();
void setupTerminalPins();

void setupUART();
void uartTask(void* pvParameters);
void sendMessage(const char *format, ...);

void terminalTimerCallback(TimerHandle_t timer_handle);
void terminalControlTask(void* pvParameters);

#endif /* TERMINAL_COMPONENT_H */
