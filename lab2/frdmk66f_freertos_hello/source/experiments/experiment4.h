#ifndef EXPERIMENT4_H
#define EXPERIMENT4_H


#include "FreeRTOS.h"
#include "timers.h"

int experiment4(void);
void hello_task(void* pvParameters);
void timerCallbackFunction(TimerHandle_t timer_handle);
void timerCallbackFunction2(TimerHandle_t timer_handle);

#endif /* EXPERIMENT4_H */
