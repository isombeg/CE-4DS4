/*
 * Motor_Control_Component_Test.c
 *
 *  Created on: Mar. 6, 2023
 *      Author: guy-jacquesisombe
 */

#include "Motor_Control_Component_Test.h"

SpeedMode_t modes[] = {MODE0, MODE1, MODE2};
SpeedDirection_t dirs[] = {FORWARD, BACKWARD};

int modes_i, dirs_i;
void test_MotorTask(){

	TimerHandle_t timer_handle = xTimerCreate("Periodic timer",
		100 / portTICK_PERIOD_MS,
		pdTRUE, // Indicates periodic
		NULL,
		testTimer_MotorTask
	);

	MotorQueueMessage_t *message = malloc(sizeof(MotorQueueMessage_t));
	message->type = SPEED_MODE;

	SpeedModePayload_t *payload = malloc(sizeof(SpeedModePayload_t));
	message->payload = (void*) payload;

	modes_i = 0; dirs_i = 0;

	BaseType_t status = xTimerStart(timer_handle, 0);
	if(status != pdPASS)
	{
		PRINTF("Couldn’t start the timer!.\r\n");
		while (1);
	}
}


void testTimer_MotorTask(TimerHandle_t handle){
	SpeedModePayload_t payload = {dirs[dirs_i], modes[modes_i]};
	MotorQueueMessage_t msg = {SPEED_MODE, (void *) &payload};

	PRINTF("[TEST - Motor Task] Mode Code: %d, Direction Code: %d\n", modes[modes_i], dirs[dirs_i]);
	payload.mode = modes[modes_i];
	payload.direction = dirs[dirs_i];

	xQueueSendToBack(motor_queue, (void*) &msg, portMAX_DELAY);

	modes_i = (modes_i+1)%3;
	dirs_i = (dirs_i+1)%2;
}
