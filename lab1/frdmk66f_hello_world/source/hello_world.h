#ifndef _HELLO_WORLD_H_
#define _HELLO_WORLD_H_

#include "fsl_ftm.h"

int problem1(void);
int problem2(void);
int problem3();
int problem4();
int problem5();

void setupPWM(ftm_chnl_t chnlNumber);
void updatePWM_dutyCycle(ftm_chnl_t channel, float dutyCycle);

void setupUART(bool interrupt_flag);
void parse_UARTInput(int* dc_speed, int* servo_angle, const char* serial_input);
int parse_UARTInput_DC(const char* serial_input);
int parse_UARTInput_Servo( const char* serial_input);

void drive_dc_and_servo(char* formatted_vals);

int parseSignedStringToInt(int signIndex, int start, int end, const char* serial_input);

void UART4_RX_TX_IRQHandler();

status_t SPI_write(uint8_t regAddress, uint8_t value);
status_t SPI_read(uint8_t regAddress, uint8_t *rxBuff, uint8_t rxBuffSize);

#endif
