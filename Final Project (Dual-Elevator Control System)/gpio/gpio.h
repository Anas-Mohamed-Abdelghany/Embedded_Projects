/**
 * @file gpio.h
 * @brief GPIO abstraction for STM32 dual-elevator system.
 */

#ifndef GPIO_H
#define GPIO_H

#include "main/main.h"
#include "exti/exti.h"

typedef enum {
    GPIO_MODE_INPUT  = 0,
    GPIO_MODE_OUTPUT = 1,
    GPIO_MODE_AF     = 2,
    GPIO_MODE_ANALOG = 3
} gpio_mode_t;

typedef enum {
    GPIO_PUPD_NONE = 0,
    GPIO_PUPD_UP   = 1,
    GPIO_PUPD_DOWN = 2
} gpio_pupd_t;

/**
 * @brief Initializes a GPIO pin using port index (0=A, 1=B, etc).
 * @param port_idx Port index
 * @param pin Pin number (0-15)
 * @param mode Operation mode
 * @param pupd Pull-up/down config
 */
void GPIO_Init_Pin(uint8_t port_idx, uint8_t pin, gpio_mode_t mode, gpio_pupd_t pupd);

/**
 * @brief Sets the alternate function for a pin.
 * @param port_idx Port index
 * @param pin Pin number
 * @param af Alternate function index (0-15)
 */
void GPIO_Set_AF(uint8_t port_idx, uint8_t pin, uint8_t af);

/**
 * @brief Writes a value to a GPIO pin.
 */
void GPIO_Write_Pin(uint8_t port_idx, uint8_t pin, bool_t state);

/**
 * @brief Reads the state of a GPIO pin.
 */
bool_t GPIO_Read_Pin(uint8_t port_idx, uint8_t pin);

#endif /* GPIO_H */
