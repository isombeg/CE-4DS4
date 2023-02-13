#include "problem1.h"

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

char* str1;
TaskHandle_t task_input_handle1;

int problem1(void)
{
	BaseType_t status;
	/* Init board hardware. */
	BOARD_InitBootPins();
	BOARD_InitBootClocks();
	BOARD_InitDebugConsole();
	status = xTaskCreate(task_input_1, "Task_input", 200, NULL, 2, &task_input_handle1);

	str1 = malloc(20*sizeof(char));

	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}

	status = xTaskCreate(task_print_input_1, "Task_print_input", 200, (void*)str1, 3, NULL);

	if (status != pdPASS)
	{
		PRINTF("Task creation failed!.\r\n");
		while (1);
	}
	vTaskStartScheduler();

	for (;;);
}

void task_input_1(void *pvParameters)
{
		PRINTF("Provide input: ");
		SCANF("%s",str1);
		vTaskDelete(task_input_handle1);

}
void task_print_input_1(void *pvParameters)
{
	//PRINTF("in task_print_input\n");
	while(str1 != NULL)
	{
		PRINTF("%s.\r\n", (char*) pvParameters);
		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}
}
