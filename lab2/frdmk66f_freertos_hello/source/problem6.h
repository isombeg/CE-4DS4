#ifndef PROBLEM6_H
#define PROBLEM6_H


#include "FreeRTOS.h"
#include "timers.h"

int problem6();
void print_task(void* pvParameters);
void print_timerCallback(TimerHandle_t timer_handle);

#endif /* PROBLEM6_H */
