#include "Terminal_Component.h"

EventGroupHandle_t event_group_global_handler;
QueueHandle_t uart_queue;
int sem_taken;

void setupTerminalComponent()//this function is called
{
	BaseType_t status;

	setupTerminalPins();

	setupUART();
	//	rc_hold_semaphore = (SemaphoreHandle_t*) malloc(2 * sizeof(SemaphoreHandle_t));
	//	rc_hold_semaphore[0] = xSemaphoreCreateBinary();//producer sem, signal to run this code
	//	rc_hold_semaphore[1] = xSemaphoreCreateBinary();//Consumer sem
    /*************** UART Task ***************/
	uart_queue = xQueueCreate(10, sizeof(char*));
	sem_taken=0;
	if (uart_queue == NULL)
	{
		PRINTF("Queue creation failed!.\r\n");
		while (1);
	}
    status = xTaskCreate(uartTask, "UART task", 200, NULL, 2, NULL);
    if (status != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1);
    }

    /*************** Terminal Control Task ***************/
    //Create Event groups
    event_group_global_handler = xEventGroupCreate();
	//Create Terminal Control Task
    status = xTaskCreate(terminalControlTask, "Terminal task", 200, NULL, 3, NULL);
    	if (status != pdPASS)
    	{
    		PRINTF("Terminal control task creation failed!.\r\n");
    		while (1);
    	}
}

void setupTerminalPins()
{
    //Configure UART pins
    CLOCK_EnableClock(kCLOCK_PortC);
    CLOCK_EnableClock(kCLOCK_PortE);
	PORT_SetPinMux(PORTC, 12U,kPORT_MuxAlt3);//rts_b
	PORT_SetPinMux(PORTC, 13U, kPORT_MuxAlt3);//cts_b
	PORT_SetPinMux(PORTC, 14U, kPORT_MuxAlt3);//rx
	PORT_SetPinMux(PORTC, 15U, kPORT_MuxAlt3);//tx
	PORT_SetPinMux(PORTE, 27U, kPORT_MuxAlt3);//
}

void sendMessage(const char *format, ...)
{
	va_list args;

	char* str = (char*)pvPortMalloc(250 * sizeof(char));

	va_start(args, format);
	vsprintf(str, format, args);

	if(xQueueSendToBack(uart_queue, (void *) &str, portMAX_DELAY) != pdPASS )
	{
		PRINTF("Send message to uart_queue failed!.\r\n");
		while (1);
	}

	va_end(args);
}

void setupUART()
{
	//Setup UART for sending and receiving
	uart_config_t config;
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 57600;
	config.enableTx = true;
	config.enableRx = true;
	config.enableRxRTS = true;
	config.enableTxCTS = true;
	UART_Init(TARGET_UART, &config, CLOCK_GetFreq(kCLOCK_BusClk));

	/******** Enable Interrupts *********/
	UART_EnableInterrupts(TARGET_UART, kUART_RxDataRegFullInterruptEnable);
	NVIC_SetPriority(UART4_RX_TX_IRQn, 2);
	EnableIRQ(UART4_RX_TX_IRQn);

}

void uartTask(void* pvParameters)
{
	char* welcome_message = "UART task started  #8\n\r";
	char* received_str;
	BaseType_t status;

	UART_WriteBlocking(TARGET_UART, welcome_message, strlen(welcome_message));

	while(1)
	{
		status = xQueueReceive(uart_queue, (void *) &received_str, portMAX_DELAY);
		if (status != pdPASS)
		{
			PRINTF("Queue Send failed!.\r\n");
			while (1);
		}
		UART_WriteBlocking(TARGET_UART, received_str, strlen(received_str));
		vPortFree(received_str);
	}
}

void UART4_RX_TX_IRQHandler()
{
	//UART ISR

	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	UART_GetStatusFlags(TARGET_UART);
	char ch = UART_ReadByte(TARGET_UART);
	switch(ch)
	{
		case 'a':
		xEventGroupSetBitsFromISR(event_group_global_handler, LEFT_BIT, &
		xHigherPriorityTaskWoken);
		break;

	case 's':
		xEventGroupSetBitsFromISR(event_group_global_handler, DOWN_BIT, &
		xHigherPriorityTaskWoken);
		break;
	case 'd':
		xEventGroupSetBitsFromISR(event_group_global_handler, RIGHT_BIT, &
		xHigherPriorityTaskWoken);
		break;
	case 'w':
		xEventGroupSetBitsFromISR(event_group_global_handler, UP_BIT, &
		xHigherPriorityTaskWoken);
		break;
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void terminalControlTask(void* pvParameters)
{
	//EventGroupHandle_t event_group = (EventGroupHandle_t)pvParameters;
	float  dc_dutyCycle;
	float servo_dutyCycle;
	EventBits_t bits;
	BaseType_t status;



	while(1)
	{
		bits = xEventGroupWaitBits(event_group_global_handler,
									LEFT_BIT | RIGHT_BIT | UP_BIT | DOWN_BIT,
									pdTRUE,
									pdFALSE,
									portMAX_DELAY);

		status = xSemaphoreTake(rc_hold_semaphore, portMAX_DELAY);

		if((bits & LEFT_BIT) == LEFT_BIT)
		{
			PRINTF("Left\r\n");
			servo_dutyCycle = .095;
			//send servo angle to the left through queue
		}

		if((bits & RIGHT_BIT) == RIGHT_BIT)
		{
			PRINTF("Right\r\n");
			servo_dutyCycle = .055;
			//send servo angle to the right through queue
		}

		if((bits & UP_BIT) == UP_BIT)
		{
			PRINTF("Up\r\n");
			dc_dutyCycle = .085;
			//send motor forward through queue
			//send LED through queue

		}

		if((bits & DOWN_BIT) == DOWN_BIT)
		{
			PRINTF("Down\r\n");
			dc_dutyCycle = .065;

		}
//		status = xQueueSendToBack(motor_queue, (void*) &dc_dutyCycle, portMAX_DELAY);
//		status = xQueueSendToBack(angle_queue, (void*) &servo_dutyCycle, portMAX_DELAY);
//		status = xQueueSendToBack(led_queue, (void*) &1500, portMAX_DELAY);
		//xSemaphoreGive(rc_hold_semaphore);
	}
}
