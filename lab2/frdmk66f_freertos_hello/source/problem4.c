// TODO: make includes

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

