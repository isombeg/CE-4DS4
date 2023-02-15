/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"
#include <ctype.h>
#include <stdio.h>
#include "event_groups.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"

/* Experiments include */
#include "experiments/experiment4.h"

/* Problems include */
#include "problem1.h"
#include "problem2.h"
#include "problem3.h"
#include "problem4.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */



int main(void){
	return problem6();
}


