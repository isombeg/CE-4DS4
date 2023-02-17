#include "problem6.h"

#include <stdio.h>
#include <ctype.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "semphr.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

SemaphoreHandle_t timer_semphr;

int problem6(){
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	timer_semphr = xSemaphoreCreateBinary();

	// Create a timer with 1 sec period. Callback signals a semaphore to a task
	TimerHandle_t timer_handle = xTimerCreate("Periodic timer",
		1000 / portTICK_PERIOD_MS,
		pdTRUE, // Indicates periodic
		NULL,
		print_timerCallback
	);

	// Create a task that prints something on the console when given the signal
	status = xTaskCreate(print_task, "Print_task", 200, NULL, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}

	// Start timer
	status = xTimerStart(timer_handle, 0);
	if(status != pdPASS)
	{
		PRINTF("Couldn’t start the timer!.\r\n");
		while (1);
	}

	// Start task scheduler
	vTaskStartScheduler();
	while (1){}
}

void print_task(void* pvParameters){
	BaseType_t status;

	while(1){
		status = xSemaphoreTake(timer_semphr, portMAX_DELAY);
		if(status != pdPASS){
			PRINTF("Failed to acquire timer_semphr\r\n");
			while (1);
		}

		PRINTF("something\n");
	}
}

void print_timerCallback(TimerHandle_t timer_handle){
	xSemaphoreGive(timer_semphr);
}

