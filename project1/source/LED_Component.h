#ifndef LED_COMPONENT_H
#define LED_COMPONENT_H

#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_ftm.h"
#include "fsl_debug_console.h"

#include "clock_config.h"
#include "board.h"
#include "fsl_gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Motor_Control_Component.h"
#include "Terminal_Component.h"

#include <stdarg.h>

#define FTM_LED 				FTM3
#define FTM_RED_CHANNEL			kFTM_Chnl_1		//Define red LED channel
#define FTM_GREEN_CHANNEL		kFTM_Chnl_5//Define green LED channel
#define FTM_BLUE_CHANNEL		kFTM_Chnl_4//Define blue LED channel
#define SLOW_SPEED_MODE 0
#define MODERATE_SPEED_MODE 1
#define FAST_SPEED_MODE 2
# define MASK 0x0000ff

extern QueueHandle_t led_queue;

void setupLEDComponent();
void setupLEDs();
void setupLEDPins();

void ledTask(void* pvParameters);

void test_timer();

void testTimer_LEDTask(TimerHandle_t handle);

#endif /* TERMINAL_COMPONENT_H */
