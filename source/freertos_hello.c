#include "FreeRTOS.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "clock_config.h"
#include "board.h"

#include "Motor_Control_Component.h"
#include "RC_Receiver_Component.h"
#include "Terminal_Component.h"
#include "LED_Component.h"
#include "Accelerometer_Component.h"

/* Test includes */

int main(void)
{
    /* Init board hardware. */
	//BOARD_InitBootPins();
    BOARD_InitBootClocks();
//
    setupMotorComponent();
    setupRCReceiverComponent();
//    setupTerminalComponent();
//    setupLEDComponent();
//    setupAccelerometerComponent();

    //test_MotorTask();
    vTaskStartScheduler();

    while(1)
    {

    }
}