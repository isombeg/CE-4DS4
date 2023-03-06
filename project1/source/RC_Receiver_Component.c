#include "RC_Receiver_Component.h"

SemaphoreHandle_t rc_hold_semaphore;
TaskHandle_t rc_task_handle;

void setupRCReceiverComponent()
{
	setupRCPins();

	setupUART_RC();

    /*************** RC Task ***************/
	//Create RC Semaphore
	//Create RC Task
}

void setupRCPins()
{
	//Configure RC pins
}

void setupUART_RC()
{
	//setup UART for RC receiver
}

void rcTask(void* pvParameters)
{
	//RC task implementation
}


