#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

/* ---- Port names ---- */
#define GPIO_A   'A'
#define GPIO_B   'B'
#define GPIO_C   'C'
#define GPIO_D   'D'

/* ---- Pin modes (MODER field, 2 bits) ---- */
#define GPIO_INPUT    0x00U   /* 00 - Input mode               */
#define GPIO_OUTPUT   0x01U   /* 01 - General-purpose output   */
#define GPIO_AF       0x02U   /* 10 - Alternate function       */
#define GPIO_ANALOG   0x03U   /* 11 - Analog (for ADC/DAC)     */

/* ---- Output type (OTYPER, 1 bit) ---- */
#define GPIO_PUSH_PULL    0x00U   /* 0 - Push-pull (default)  */
#define GPIO_OPEN_DRAIN   0x01U   /* 1 - Open-drain           */

/* ---- Pull-up/down (PUPDR field, 2 bits) ---- */
#define GPIO_NO_PULL    0x00U   /* 00 - No pull               */
#define GPIO_PULL_UP    0x01U   /* 01 - Pull-up resistor      */
#define GPIO_PULL_DOWN  0x02U   /* 10 - Pull-down resistor    */

/* ---- Logic levels ---- */
#define HIGH   0x01U
#define LOW    0x00U

/* ---- Status codes ---- */
#define GPIO_OK    0x00U
#define GPIO_NOK   0x01U

/* ---- Public API ---- */
void    Gpio_Init    (uint8_t PortName, uint8_t PinNumber,
                      uint8_t PinMode,  uint8_t DefaultState);

uint8_t Gpio_WritePin(uint8_t PortName, uint8_t PinNumber, uint8_t Data);

uint8_t Gpio_ReadPin (uint8_t PortName, uint8_t PinNumber);

/* Set alternate function number for a pin (AFR register) */
void    Gpio_SetAF   (uint8_t PortName, uint8_t PinNumber, uint8_t AFNumber);

#endif /* GPIO_H */
