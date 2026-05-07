#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>

#define GPIO_INPUT  0x00U
#define GPIO_OUTPUT 0x01U
#define HIGH 0x01U
#define LOW  0x00U

void    Gpio_WritePin(char PortName, uint8_t PinNumber, uint8_t Data);
uint8_t Gpio_ReadPin(char PortName, uint8_t PinNumber);
void    Gpio_Init(char PortName, uint8_t PinNumber, uint8_t PinMode, uint8_t DefaultState);

#endif