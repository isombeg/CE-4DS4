#include "problem4.h"

#include <stdio.h>
#include <ctype.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "event_groups.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

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


int problem4_2(){
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	// Setup task input
	Task_Input* taskInput = malloc(sizeof(Task_Input));

	taskInput->sems = malloc(2*sizeof(SemaphoreHandle_t));
	taskInput->sems[0] = xSemaphoreCreateBinary(); //Producer_sem
	taskInput->sems[1] = xSemaphoreCreateBinary(); //consumer_sem

	status = xTaskCreate(dir_producer_sem, "producer", 200, (void*)taskInput, 2, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(dir_consumer_sem, "consumer", 200, (void*)taskInput, 3, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();
	while (1)
	{}

	return 0;
}

void dir_producer_sem(void* pvParameters){
	Task_Input* task_input = (Task_Input *)pvParameters;
	SemaphoreHandle_t producer_semaphore = task_input->sems[0];
	SemaphoreHandle_t consumer_semaphore = task_input->sems[1];
	BaseType_t status;
	char c;
	while(1)
	{

		status = xSemaphoreTake(consumer_semaphore, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Failed to acquire consumer_semaphore\r\n");
			while (1);
		}

		PRINTF("Input direction: ");
		SCANF("%c", &c);
		switch(c)
		{
		case 'a':
			task_input->dir_bits = LEFT_BIT;
			break;
		case 's':
			task_input->dir_bits = DOWN_BIT;
			break;
		case 'd':
			task_input->dir_bits = RIGHT_BIT;
			break;
		case 'w':
			task_input->dir_bits = UP_BIT;
			break;
		}

		xSemaphoreGive(producer_semaphore);
	}
}

void dir_consumer_sem(void* pvParameters){
	Task_Input* task_input = (Task_Input *)pvParameters;
	SemaphoreHandle_t producer_semaphore = task_input->sems[0];
	SemaphoreHandle_t consumer_semaphore = task_input->sems[1];
	BaseType_t status;
	short bits;

	xSemaphoreGive(consumer_semaphore);

	while(1)
	{
		status = xSemaphoreTake(producer_semaphore, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Failed to acquire producer1_semaphore\r\n");
			while (1);
		}

		bits = task_input->dir_bits;

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

		xSemaphoreGive(consumer_semaphore);
	}
}


TaskHandle_t userInput_producerEvent_handle;

void problem4_3(){
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();

	EventGroup_TaskInput* task_input = malloc(sizeof(EventGroup_TaskInput));
	task_input->event_group = xEventGroupCreate();
	task_input->user_input = malloc(20*sizeof(char));

	status = xTaskCreate(userInput_producerEvent, "producer", 200, (void*)task_input, 2, &userInput_producerEvent_handle);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(userInput_consumerEvent_1, "consumer1", 200, (void*)task_input, 3, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	status = xTaskCreate(userInput_consumerEvent_2, "consumer2", 200, (void*)task_input, 3, NULL);
	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();
	while (1)
	{}
}

void userInput_producerEvent(void* pvParameters){
	EventGroup_TaskInput* input = (EventGroup_TaskInput*) pvParameters;

	PRINTF("Provide input: ");
	SCANF("%s", input->user_input);

	xEventGroupSetBits(input->event_group, ARBITRARY_BIT);
	vTaskDelete(userInput_producerEvent_handle);
}

void userInput_consumerEvent_1(void* pvParameters){
	EventGroup_TaskInput* input = (EventGroup_TaskInput*) pvParameters;

	// Wait for bits
	xEventGroupWaitBits(input->event_group,
		ARBITRARY_BIT,
		pdTRUE,
		pdFALSE,
		portMAX_DELAY
	);

	while(1){
		// Print received input
		PRINTF("Received Value = %s\r\n", input->user_input);
		// Go into block state for 1 second
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}

void userInput_consumerEvent_2(void* pvParameters){
	EventGroup_TaskInput* input = (EventGroup_TaskInput*) pvParameters;

	// Initialize needed vars
	int j = 0;
	char upperCaseStr[20];

	// Wait for event group bits
	xEventGroupWaitBits(input->event_group,
		ARBITRARY_BIT,
		pdTRUE,
		pdFALSE,
		portMAX_DELAY
	);

	while(1){
		// Convert user_input to upper case
		while(j < 20 && input->user_input[j]){
			upperCaseStr[j++] = toupper(input->user_input[j]);
		}

		// End string
		if(j < 20) upperCaseStr[j] = 0;

		// Display upper case string
		PRINTF("Received Value (Upper Case) = %s\r\n ", upperCaseStr);
		//Go into block state for 1 second
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

}

