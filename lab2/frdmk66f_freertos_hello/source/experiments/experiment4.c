#include "experiment4.h"

#include <stdio.h>
#include <ctype.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

int experiment4(void)
{
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	status = xTaskCreate(hello_task, "Hello_task", 200, NULL, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}

	TimerHandle_t timer_handle = xTimerCreate("Single Shot timer",
		5000 / portTICK_PERIOD_MS,
		pdFALSE, // Indicates one-shot
		NULL,
		timerCallbackFunction
	);

	status = xTimerStart(timer_handle, 0);
	if(status != pdPASS)
	{
		PRINTF("Couldn’t start the timer!.\r\n");
		while (1);
	}

	TimerHandle_t timer_handle2 = xTimerCreate("Periodic timer",
		1000 / portTICK_PERIOD_MS,
		pdTRUE, // Indicates periodic
		NULL,
		timerCallbackFunction2
	);

	status = xTimerStart(timer_handle2, 0);
	if(status != pdPASS)
	{
		PRINTF("Couldn’t start the timer!.\r\n");
		while (1);
	}

	vTaskStartScheduler();
	while (1){}
}

void hello_task(void* pvParameters){
	while(1){
		PRINTF("Hello world");
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void timerCallbackFunction(TimerHandle_t timer_handle)
{
	PRINTF("Hello from the single shot timer callback.\r\n");
}

void timerCallbackFunction2(TimerHandle_t timer_handle)
{
	static int counter = 0;

	PRINTF("Hello from the periodic timer callback. Counter = %d\r\n", counter);
	counter++;
	if(counter >= 10)
		xTimerStop(timer_handle, 0);
}
