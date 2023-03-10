/*
 * Motor_Control_Component_Test.h
 *
 *  Created on: Mar. 6, 2023
 *      Author: guy-jacquesisombe
 */

#ifndef MOTOR_CONTROL_COMPONENT_TEST_H_
#define MOTOR_CONTROL_COMPONENT_TEST_H_

#include "Motor_Control_Component.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

void test_MotorTask();
void testTimer_MotorTask(TimerHandle_t);

void test_PositionTask();
void testTimer_PositionTask(TimerHandle_t);

#endif /* MOTOR_CONTROL_COMPONENT_TEST_H_ */
