#include "Accelerometer_Component.h"

static fxos_handle_t fxosHandle;
static uint8_t data_scale;

void setupAccelerometerComponent()
{
	setupAccelerometerPins();
	voltageRegulator_enable();
	accelerometer_enable();

	spi_init();

    /*************** Accelerometer Task ***************/
	//Create Accelerometer Task
}

void setupAccelerometerPins()
{
	//Initialize Accelerometer Pins
}

void voltageRegulator_enable()
{
	//Enable voltage Regulator
}

void accelerometer_enable()
{
	//Enable accelerometer
}

void spi_init()
{
    //Initialize SPI
}

status_t SPI_Send(uint8_t regAddress, uint8_t value)
{

}

status_t SPI_receive(uint8_t regAddress, uint8_t *rxBuff, uint8_t rxBuffSize)
{

}

void accelerometerTask(void* pvParameters)
{
	//Accelerometer task implementation
}
