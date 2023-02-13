#include "problem3.h"

#include <stdio.h>
#include <ctype.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

TaskHandle_t task_input_handle;
char* str;

int problem3(){
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	str = malloc(20*sizeof(char));
	SemaphoreHandle_t* semaphores = (SemaphoreHandle_t*) malloc(3 * sizeof(
	SemaphoreHandle_t));
	semaphores[0] = xSemaphoreCreateBinary(); //Producer1_sem
	semaphores[1] = xSemaphoreCreateBinary(); //Producer2_sem
	semaphores[2] = xSemaphoreCreateCounting(2, 2); //consumer_sem
	status = xTaskCreate(producer_sem, "producer", 200, (void*)semaphores, 2, &task_input_handle);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(consumer1_sem, "consumer", 200, (void*)semaphores, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(consumer2_sem, "consumer", 200, (void*)semaphores, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();
	while (1)
	{}
}

void producer_sem(void* pvParameters)
{
	SemaphoreHandle_t* semaphores = (SemaphoreHandle_t*)pvParameters;
	SemaphoreHandle_t producer1_semaphore = semaphores[0];
	SemaphoreHandle_t producer2_semaphore = semaphores[1];
	SemaphoreHandle_t consumer_semaphore = semaphores[2];
	BaseType_t status1, status2;

	status1 = xSemaphoreTake(consumer_semaphore, portMAX_DELAY);
	status2 = xSemaphoreTake(consumer_semaphore, portMAX_DELAY);
	if (status1 != pdPASS || status2 != pdPASS)
	{
		PRINTF("Failed to acquire consumer_semaphore\r\n");
		while (1);
	}

	PRINTF("Provide input: ");
	SCANF("%s", str);
	xSemaphoreGive(producer1_semaphore);
	xSemaphoreGive(producer2_semaphore);
	vTaskDelete(task_input_handle);
}

void consumer1_sem(void* pvParameters)
{
	SemaphoreHandle_t* semaphores = (SemaphoreHandle_t*)pvParameters;
	SemaphoreHandle_t producer1_semaphore = semaphores[0];
	SemaphoreHandle_t consumer_semaphore = semaphores[2];
	BaseType_t status;
	xSemaphoreGive(consumer_semaphore);
		status = xSemaphoreTake(producer1_semaphore, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Failed to acquire producer1_semaphore\r\n");
			while (1);
		}
	while(1)
	{
		PRINTF("Received Value = %s\r\n", str);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void consumer2_sem(void* pvParameters)
	{
	int j = 0;
	char upperSTR[20];
	SemaphoreHandle_t* semaphores = (SemaphoreHandle_t*)pvParameters;
	SemaphoreHandle_t producer2_semaphore = semaphores[1];
	SemaphoreHandle_t consumer_semaphore = semaphores[2];
	BaseType_t status;
	xSemaphoreGive(consumer_semaphore);
		status = xSemaphoreTake(producer2_semaphore, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Failed to acquire producer2_semaphore\r\n");
			while (1);
		}

	while(1)
	{
		while (str[j]){
				upperSTR[j] = toupper(str[j]);
				j++;
			}
		upperSTR[j] = '\0';
		PRINTF("Received Value upper = %s\r\n ", upperSTR);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
