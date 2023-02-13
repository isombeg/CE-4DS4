/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include <ctype.h>
#include <stdio.h>
#include "event_groups.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define LEFT_BIT (1 << 0)
#define RIGHT_BIT (1 << 1)
#define UP_BIT (1 << 2)
#define DOWN_BIT (1 << 3)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */

typedef struct {
	SemaphoreHandle_t* sems;
	char* dir;
} Task_Input;

char* str;
TaskHandle_t task_input_handle, consumer_handle;

void task_input_1(void *pvParameters)
{
		PRINTF("Provide input: ");
		SCANF("%s",str);
		vTaskDelete(task_input_handle);

}
void task_print_input_1(void *pvParameters)
{
	//PRINTF("in task_print_input\n");
	while(str != NULL)
	{
		PRINTF("%s.\r\n", (char*) pvParameters);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
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

int counter = 0;
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

void producer_event(void* pvParameters)
{
	EventGroupHandle_t event_group = (EventGroupHandle_t)pvParameters;
	BaseType_t status;
	char c;
	while(1)
	{
		scanf("%c", &c);
		switch(c)
		{
		case 'a':
			xEventGroupSetBits(event_group, LEFT_BIT);
			break;
		case 's':
			xEventGroupSetBits(event_group, DOWN_BIT);
			break;
		case 'd':
			xEventGroupSetBits(event_group, RIGHT_BIT);
			break;
		case 'w':
			xEventGroupSetBits(event_group, UP_BIT);
			break;
		}
	}
}

void consumer_event(void* pvParameters)
{
	EventGroupHandle_t event_group = (EventGroupHandle_t)pvParameters;
	EventBits_t bits;
	while(1)
	{
		bits = xEventGroupWaitBits(event_group,
		LEFT_BIT | RIGHT_BIT | UP_BIT | DOWN_BIT,
		pdTRUE,
		pdFALSE,
		portMAX_DELAY);
		if((bits & LEFT_BIT) == LEFT_BIT)
		{
			PRINTF("Left\r\n");
		}
		if((bits & RIGHT_BIT) == RIGHT_BIT)
		{
			PRINTF("Right\r\n");
		}
		if((bits & UP_BIT) == UP_BIT)
		{
			PRINTF("Up\r\n");
		}
		if((bits & DOWN_BIT) == DOWN_BIT)
		{
			PRINTF("Down\r\n");
		}
	}
}

//void dir_producer_sem(void* pvParameters){
//	Task_Input* task_input = (Task_Input *)pvParameters;
//	BaseType_t status;
//	char c;
//	status = xSemaphoreTake(task_input->sems[1], portMAX_DELAY);
//	if (status != pdPASS)
//	{
//		PRINTF("Failed to acquire consumer_semaphore\r\n");
//		while (1);
//	}
//	while(1)
//	{
//		scanf("%c", task_input->dir);
//		xSemaphoreGive(task_input->sems[0]);
//		vTaskDelay(1000 / portTICK_PERIOD_MS);
//	}
//}

//void dir_consumer_sem(void* pvParameters){
//	Task_Input* task_input = (Task_Input *)pvParameters;
//	SemaphoreHandle_t producer_semaphore = task_input->sems[0];
//	SemaphoreHandle_t consumer_semaphore = task_input->sems[0];
//	BaseType_t status;
//	xSemaphoreGive(consumer_semaphore);
//	status = xSemaphoreTake(producer_semaphore, portMAX_DELAY);
//	if (status != pdPASS)
//	{
//		PRINTF("Failed to acquire producer1_semaphore\r\n");
//		while (1);
//	}
//	while(1)
//		{
//			bits = xEventGroupWaitBits(event_group,
//			LEFT_BIT | RIGHT_BIT | UP_BIT | DOWN_BIT,
//			pdTRUE,
//			pdFALSE,
//			portMAX_DELAY);
//			if((bits & LEFT_BIT) == LEFT_BIT)
//			{
//				PRINTF("Left\r\n");
//			}
//			if((bits & RIGHT_BIT) == RIGHT_BIT)
//			{
//				PRINTF("Right\r\n");
//			}
//			if((bits & UP_BIT) == UP_BIT)
//			{
//				PRINTF("Up\r\n");
//			}
//			if((bits & DOWN_BIT) == DOWN_BIT)
//			{
//				PRINTF("Down\r\n");
//			}
//		}
//}

int problem1(void)
{
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	status = xTaskCreate(task_input_1, "Task_input", 200, NULL, 2, &task_input_handle);

	str = malloc(20*sizeof(char));

	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}

	status = xTaskCreate(task_print_input_1, "Task_print_input", 200, (void*)str, 3, NULL);

	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();

	for (;;);
}

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

int problem4(){
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	EventGroupHandle_t event_group = xEventGroupCreate();
	status = xTaskCreate(producer_event, "producer", 200, (void*)event_group, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(consumer_event, "consumer", 200, (void*)event_group, 3, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();
	while (1)
	{}
}
//
//void problem4_2(){
//	BaseType_t status;
//	/* Init board hardware. */
//	BOARD_InitBootPins();
//	BOARD_InitBootClocks();
//	BOARD_InitDebugConsole();
//	char dir;
//	SemaphoreHandle_t* semaphores = (SemaphoreHandle_t*) malloc(2 * sizeof(
//	SemaphoreHandle_t));
//	semaphores[0] = xSemaphoreCreateBinary(); //Producer1_sem
//	semaphores[1] = xSemaphoreCreateBinary(); //consumer_sem
//	Task_Input taskInput = {semaphores, &dir};
//
//	status = xTaskCreate(producer_sem, "producer", 200, (void*)taskInput, 2, NULL);
//	if (status != pdPASS)
//	{
//		PRINTF("Task creation failed!.\r\n");
//		while (1);
//	}
//	status = xTaskCreate(consumer1_sem, "consumer", 200, (void*)taskInput, 2, NULL);
//	if (status != pdPASS)
//	{
//		PRINTF("Task creation failed!.\r\n");
//		while (1);
//	}
//	vTaskStartScheduler();
//	while (1)
//	{}
//}

int main(void){
	return problem1();
}


