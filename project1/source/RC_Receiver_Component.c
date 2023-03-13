#include "RC_Receiver_Component.h"
#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

SemaphoreHandle_t rc_hold_semaphore;
TaskHandle_t rc_task_handle;

typedef struct {
uint16_t header;
uint16_t ch1;
uint16_t ch2;
uint16_t ch3;
uint16_t ch4;
uint16_t ch5;
uint16_t ch6;
uint16_t ch7;
uint16_t ch8;
} RC_Values;

void setupRCReceiverComponent() //Like a main() function
{
	setupRCPins();
	setupUART_RC();
	BaseType_t status;

    /*************** RC Task ***************/
	//Create RC Semaphore FOR BONUS!!!!
//	rc_hold_semaphore = (SemaphoreHandle_t*) malloc(sizeof(SemaphoreHandle_t));
//	rc_hold_semaphore = xSemaphoreCreateBinary();//producer sem, signal to run this code


	//Create RC Task

	status = xTaskCreate(rcTask, "remote", 200, NULL, 3, &rc_task_handle);
	if (status != pdPASS)
	{
		PRINTF("RC Task creation failed!.\r\n");
		while (1);
	}

}

void setupRCPins()
{
	//Configure RC pins
	CLOCK_EnableClock(kCLOCK_PortC);

	PORT_SetPinMux(PORTC, 3U,kPORT_MuxAlt3); // ! probs disables
}

void setupUART_RC()
{
	uart_config_t config;
	//setup UART for RC receiver
	UART_GetDefaultConfig(&config);
	config.baudRate_Bps = 115200;
	config.enableTx = false;
	config.enableRx = true;
	UART_Init(UART1, &config, CLOCK_GetFreq(kCLOCK_CoreSysClk));
}

void rcTask(void* pvParameters)
{
	//RC task implementation
	//float  dc_dutyCycle;
	//float servo_dutyCycle;
	int direction,dc_speed,servo_angle;

	float * dc_dutyCycle = malloc(sizeof(float));
	float * servo_dutyCycle = malloc(sizeof(float));

	int *speed_ptr, *angle_ptr, *led_ptr;


	BaseType_t status;
	RC_Values rc_values;

	uint8_t* ptr = (uint8_t*) &rc_values;
//	printf("in rc task\n");


	while (1)
	{
//		status = xSemaphoreTake(rc_hold_semaphore, portMAX_DELAY);
//
//		if (status1 != pdPASS)
//		{
//			PRINTF("Failed to acquire consumer_semaphore\r\n");
//			while (1);
//		}

		UART_ReadBlocking(UART1, ptr, 1);
		if(*ptr != 0x20)
			continue;
		UART_ReadBlocking(UART1, &ptr[1], sizeof(rc_values) - 1);

		if(rc_values.header == 0x4020)
		{
			speed_ptr = malloc(sizeof(int));
			angle_ptr = malloc(sizeof(int));
			led_ptr = malloc(sizeof(int));

//			PRINTF("[RC Task] CH 1 = %d\t", rc_values.ch1); //Angle
//			PRINTF("CH 3 = %d\t", rc_values.ch3); //Speed
//			PRINTF("CH 6 = %d\t", rc_values.ch6); // Direction
//			PRINTF("CH 7 = %d\t\r\n", rc_values.ch7); // Mode //1000 = 0, MAX = 2000
			(rc_values.ch6==2000) ? (direction = -1) : (direction = 1);

//			servo_angle = (45*(rc_values.ch1-1500))/500;
			*angle_ptr = rc_to_angle(rc_values);


			if(rc_values.ch7 == 2000){
//				dc_speed=.5*direction * (100*(rc_values.ch3-1000))/1000;
				*speed_ptr = rc_to_speed(rc_values, direction, MODE2_SPEED_SCALE);
				*led_ptr = 0;
			}
			if(rc_values.ch7 == 1500){
//				dc_speed=.75*direction * (100*(rc_values.ch3-1000))/1000;
				*speed_ptr = rc_to_speed(rc_values, direction, MODE1_SPEED_SCALE);
				*led_ptr = 1;
			}
			if(rc_values.ch7 == 1000){
//				dc_speed=1*direction * (100*(rc_values.ch3-1000))/1000;
				*speed_ptr = rc_to_speed(rc_values, direction, MODE0_SPEED_SCALE);
				*led_ptr = 2;
			}

			// ! making it drive motor the same way as the tests to see if it works
//			*dc_dutyCycle =  dc_speed_to_dutycycle(dc_speed); //dc_speed* 0.00025f + 0.070;
//			*servo_dutyCycle = servo_angle * 0.00025f + 0.065;//needs to be double checked
//			PRINTF("[RC Task] Sending to MOTOR COMPONENT: Servo Angle: %d\t DC Speed: %d\r\n",*angle_ptr,*speed_ptr);
			xQueueSendToBack(motor_queue, (void*) speed_ptr, portMAX_DELAY);
			xQueueSendToBack(angle_queue, (void*) angle_ptr, portMAX_DELAY);

			free(speed_ptr); free(angle_ptr); free(led_ptr);
			//printf("sent to queue");
			status = xQueueSendToBack(led_queue, (void*) led_ptr, portMAX_DELAY);
//			vTaskDelay(50/portTICK_PERIOD_MS);
		}

		//xSemaphoreGive(rc_hold_semaphore);
	}



}


