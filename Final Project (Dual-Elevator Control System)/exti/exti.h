/**
 * @file exti.h
 * @brief EXTI driver for STM32F401VE.
 */

#ifndef EXTI_H
#define EXTI_H

#include "main/main.h"



/* EXTI Trigger edge */
typedef enum {
    EXTI_TRIGGER_RISING = 0,
    EXTI_TRIGGER_FALLING,
    EXTI_TRIGGER_BOTH
} exti_trigger_t;

/**
 * @brief Configures EXTI for a specific pin.
 * @param port_idx Port index (0=A, 1=B, 2=C, etc.)
 * @param pin Pin number (0-15)
 * @param trigger Trigger edge
 */
void EXTI_Init_Pin(uint8_t port_idx, uint8_t pin, exti_trigger_t trigger);

/**
 * @brief Enables EXTI interrupt in NVIC.
 * @param pin Pin number (to determine IRQn)
 * @param priority Preemption priority
 */
void EXTI_Enable_IRQ(uint8_t pin, uint8_t priority);

/**
 * @brief Clears EXTI pending bit.
 * @param pin Pin number
 */
void EXTI_Clear_Pending(uint8_t pin);

#endif /* EXTI_H */
