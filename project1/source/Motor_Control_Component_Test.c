/*
 * Motor_Control_Component_Test.c
 *
 *  Created on: Mar. 6, 2023
 *      Author: guy-jacquesisombe
 */

#include "Motor_Control_Component_Test.h"

int speed;
void test_MotorTask(){

	TimerHandle_t timer_handle = xTimerCreate("Periodic timer",
		150 / portTICK_PERIOD_MS,
		pdTRUE, // Indicates periodic
		NULL,
		testTimer_MotorTask
	);

	speed = -90;
	BaseType_t status = xTimerStart(timer_handle, 0);
	if(status != pdPASS)
	{
		PRINTF("Couldn’t start the timer!.\r\n");
		while (1);
	}
}


void testTimer_MotorTask(TimerHandle_t handle){
	int * new_speed_p = malloc(sizeof(int));
	*new_speed_p = speed;
	PRINTF("[TEST - Motor Task] speed: %d\r\n", speed);
	xQueueSendToBack(motor_queue, (void*) new_speed_p, portMAX_DELAY);

	speed += 10;
	if(speed == 100) 
		speed = -90;

}

int angle;
void test_PositionTask(){

	TimerHandle_t timer_handle = xTimerCreate(
		"Position Testing Periodic Timer",
		30 / portTICK_PERIOD_MS,
		pdTRUE,
		NULL,
		testTimer_PositionTask
	);

	angle = -45;

	if((xTimerStart(timer_handle, 0)) != pdPASS){
		PRINTF("Couldn't start the timer\r\n");
		while(1);
	}
}

void testTimer_PositionTask(TimerHandle_t handle){
	int *new_angle_ptr = malloc(sizeof(int));
	*new_angle_ptr = angle;
	PRINTF("[TEST - Position Task] angle: %d\r\n", angle);
	xQueueSendToBack(angle_queue, (void*) new_angle_ptr, portMAX_DELAY);

	angle += 10;
	if(angle > 45) 
		angle = -45;
}
