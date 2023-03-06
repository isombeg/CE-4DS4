#ifndef ACCELEROMETER_COMPONENT_H
#define ACCELEROMETER_COMPONENT_H

#include "math.h"
#include "pin_mux.h"
#include "fsl_port.h"
#include "fsl_dspi.h"
#include "fsl_fxos.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "Motor_Control_Component.h"
#include "Terminal_Component.h"

void setupAccelerometerComponent();

void setupAccelerometerPins();
void voltageRegulator_enable();
void accelerometer_enable();
void spi_init();

void accelerometerTask(void* pvParameters);

status_t SPI_Send(uint8_t regAddress, uint8_t value);
status_t SPI_receive(uint8_t regAddress, uint8_t *rxBuff, uint8_t rxBuffSize);


#endif /* ACCELEROMETER_COMPONENT_H */
