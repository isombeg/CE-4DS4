#include "LED_Component.h"

QueueHandle_t led_queue;
BaseType_t status;

int current_speed_mode = SLOW_SPEED_MODE;
int new_speed_mode;
void pwm_setup(ftm_chnl_t chnlNumber){
  ftm_config_t ftmInfo;
  ftm_chnl_pwm_signal_param_t ftmParam;

  ftmParam.chnlNumber = chnlNumber;
  ftmParam.level = kFTM_HighTrue;
  ftmParam.dutyCyclePercent = 0;
  ftmParam.firstEdgeDelayPercent = 0U;
  ftmParam.enableComplementary = false;
  ftmParam.enableDeadtime = false;

  FTM_GetDefaultConfig(&ftmInfo);

  FTM_Init(FTM3, &ftmInfo);
  FTM_SetupPwm(FTM3, &ftmParam, 1U, kFTM_EdgeAlignedPwm, 5000U, CLOCK_GetFreq(kCLOCK_BusClk));
  FTM_StartTimer(FTM3, kFTM_SystemClock);
}

void hex_to_dutycycle(int rgb_input, int* rgb_vals){
  rgb_vals[0] = 100*((rgb_input >> 16) & MASK)/255;
  rgb_vals[1] = 100*((rgb_input >> 8) & MASK)/255;
  rgb_vals[2] = 100*(rgb_input & MASK)/255;
}

void setupLEDComponent()
{
	setupLEDPins();

	setupLEDs();

    /*************** LED Task ***************/
	//Create LED Queue
	led_queue = xQueueCreate(10,sizeof(int));
	if (led_queue == NULL)
	{
	PRINTF("Queue creation failed!.\r\n");
	while (1);
	}
	//Create LED Task
	xTaskCreate(ledTask,"LED Task",200,(void*)led_queue,2,NULL);

}


void setupLEDPins()
{
	//Configure LED pins
	CLOCK_EnableClock(kCLOCK_PortD);
	   /* PORTC9 (pin D7) is configured as PTC9 */
	PORT_SetPinMux(PORTB, 16U, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTB, 17U, kPORT_MuxAlt3);
	PORT_SetPinMux(PORTC, 9U, kPORT_MuxAlt3);//red led
	PORT_SetPinMux(PORTC, 8U, kPORT_MuxAlt3);//blue led
	PORT_SetPinMux(PORTD, 1U, kPORT_MuxAlt4);// green led

}

void setupLEDs()
{
	//Initialize PWM for the LEDs
	pwm_setup(FTM_RED_CHANNEL);
	pwm_setup(FTM_GREEN_CHANNEL);
	pwm_setup(FTM_BLUE_CHANNEL);

}

void test_timer(){

	TimerHandle_t timer_handle = xTimerCreate("Periodic timer",
		200 / portTICK_PERIOD_MS,
		pdTRUE, // Indicates periodic
		NULL,
		testTimer_LEDTask
	);
	new_speed_mode = 0;
	BaseType_t status = xTimerStart(timer_handle, 0);
	if(status != pdPASS)
	{
		PRINTF("Couldn’t start the timer!.\r\n");
		while (1);
	}
}

void testTimer_LEDTask(TimerHandle_t handle){
	xQueueSendToBack(led_queue, (void*) &new_speed_mode, portMAX_DELAY);

	new_speed_mode += 1;
	if(new_speed_mode == 4)
		new_speed_mode = 0;

}

void ledTask(void* pvParameters)
{
	//LED task implementation
	int rgb_val;
	int* rgb_input = malloc(3*sizeof(int));
	while(1){
		//wait for the next speed mode command from the queue
		int speed_mode_command;
		if(xQueueReceive(led_queue, &speed_mode_command, portMAX_DELAY)){
			//switch to the desired speed mode based on the command
			switch(speed_mode_command){
			// Update the RGB LED based on the current speed mode
			case 0:
				current_speed_mode = SLOW_SPEED_MODE;
				//Turn on the LED with green colour
				rgb_val = 0x00FF00;
				hex_to_dutycycle(rgb_val,rgb_input);
				FTM_UpdatePwmDutycycle(FTM3, FTM_RED_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[0]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_GREEN_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[1]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_BLUE_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[2]);
				FTM_SetSoftwareTrigger(FTM3, true);
				break;
			case 1:
				current_speed_mode = MODERATE_SPEED_MODE;
				// Turn on the LED with an orange or yellow colour
				PRINTF("yellow/n");
				rgb_val = 0xFFFF00;
				hex_to_dutycycle(rgb_val,rgb_input);
				FTM_UpdatePwmDutycycle(FTM3, FTM_RED_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[0]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_GREEN_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[1]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_BLUE_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[2]);
				FTM_SetSoftwareTrigger(FTM3, true);
				break;
			case 2:
				current_speed_mode = FAST_SPEED_MODE;
				//Turn on the LED with a red colour
				//PRINTF("red/n");
				rgb_val = 0xFF0000;
				hex_to_dutycycle(rgb_val,rgb_input);
				FTM_UpdatePwmDutycycle(FTM3, FTM_RED_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[0]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_GREEN_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[1]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_BLUE_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[2]);
				FTM_SetSoftwareTrigger(FTM3, true);
				break;
			default:
				// Invalid command, do nothing
				rgb_val = 0x000000;
				hex_to_dutycycle(rgb_val,rgb_input);
				FTM_UpdatePwmDutycycle(FTM3, FTM_RED_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[0]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_GREEN_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[1]);
				FTM_UpdatePwmDutycycle(FTM3, FTM_BLUE_CHANNEL, kFTM_EdgeAlignedPwm, rgb_input[2]);
				FTM_SetSoftwareTrigger(FTM3, true);
				break;

			}
		}
	}
}


