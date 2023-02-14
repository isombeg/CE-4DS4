#ifndef PROBLEM4_H
#define PROBLEM4_H

#include "FreeRTOS.h"
#include "semphr.h"
#include "event_groups.h"

#define LEFT_BIT (1 << 0)
#define RIGHT_BIT (1 << 1)
#define UP_BIT (1 << 2)
#define DOWN_BIT (1 << 3)

#define ARBITRARY_BIT (1 << 0)

typedef struct {
	SemaphoreHandle_t* sems;
	short dir_bits;
} Task_Input;

typedef struct {
	EventGroupHandle_t event_group;
	char* user_input;
} EventGroup_TaskInput;

int problem4();
void producer_event(void* pvParameters);
void consumer_event(void* pvParameters);

int problem4_2();
void dir_producer_sem(void* pvParameters);
void dir_consumer_sem(void* pvParameters);

void problem4_3();
void userInput_producerEvent(void* pvParameters);
void userInput_consumerEvent_1(void* pvParameters);
void userInput_consumerEvent_2(void* pvParameters);

#endif /* PROBLEM4_H */
