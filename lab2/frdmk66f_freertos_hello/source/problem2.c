#include "problem2.h"

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

TaskHandle_t consumer_handle;

int problem2(){
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	QueueHandle_t queue1 = xQueueCreate(1, sizeof(char*));
	if (queue1 == NULL)
	{
		PRINTF("Queue creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(producer_queue, "producer", 200, (void*)queue1, 2, &consumer_handle);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(consumer_queue, "consumer", 200, (void*)queue1, 3, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();
	while (1)
	{}
}

void producer_queue(void* pvParameters)
{
	QueueHandle_t queue1 = (QueueHandle_t)pvParameters;
	BaseType_t status;
	char* input_str = malloc(20*sizeof(char));

	PRINTF("Provide input: ");
	SCANF("%s",input_str);
	status = xQueueSendToBack(queue1, (void*) &input_str, portMAX_DELAY);
	vTaskDelete(consumer_handle);
}

void consumer_queue(void* pvParameters)
{
	QueueHandle_t queue1 = (QueueHandle_t)pvParameters;
	BaseType_t status;
	char* input_str;
	status = xQueueReceive(queue1, (void *) &input_str, portMAX_DELAY);
	if (status != pdPASS)
	{
		PRINTF("Queue Receive failed!.\r\n");
		while (1);
	}
	while(1)
	{
		PRINTF("Received Value = %s\r\n", input_str);
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}
