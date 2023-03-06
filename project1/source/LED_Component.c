#include "LED_Component.h"

QueueHandle_t led_queue;

void setupLEDComponent()
{
	setupLEDPins();

	setupLEDs();

    /*************** LED Task ***************/
	//Create LED Queue
	//Create LED Task
}

void setupLEDPins()
{
	//Configure LED pins
}

void setupLEDs()
{
	//Initialize PWM for the LEDs
}

void ledTask(void* pvParameters)
{
	//LED task implementation
}
