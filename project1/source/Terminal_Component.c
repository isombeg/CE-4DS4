#include "Terminal_Component.h"

EventGroupHandle_t event_group;
QueueHandle_t uart_queue;

void setupTerminalComponent()
{
	BaseType_t status;

	setupTerminalPins();

	setupUART();

    /*************** UART Task ***************/
	uart_queue = xQueueCreate(10, sizeof(char*));
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
	//Create Terminal Control Task
}

void setupTerminalPins()
{
    //Configure UART pins
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
}

void uartTask(void* pvParameters)
{
	char* welcome_message = "UART task started\n\r";
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
}

void terminalControlTask(void* pvParameters)
{
	//Terminal Control Task implementation
}
