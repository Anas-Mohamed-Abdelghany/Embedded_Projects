/**
 * @file rcc.h
 * @brief Reset and Clock Control module for STM32F401VE.
 */

#ifndef RCC_H
#define RCC_H

#include "main/main.h"
#include "exti/exti.h"

/**
 * @brief Initializes the system clock to 16MHz HSI and enables peripheral clocks.
 */
void RCC_Init(void);

/**
 * @brief Enables clock for a specific GPIO port.
 * @param port GPIO port (A, B, C, D, E)
 */
void RCC_GPIO_Enable(char port);

/**
 * @brief Enables clock for SPI1.
 */
void RCC_SPI1_Enable(void);

/**
 * @brief Enables clock for USART1.
 */
void RCC_USART1_Enable(void);

/**
 * @brief Enables clock for TIM2 and TIM3.
 */
void RCC_Timers_Enable(void);

#endif /* RCC_H */
