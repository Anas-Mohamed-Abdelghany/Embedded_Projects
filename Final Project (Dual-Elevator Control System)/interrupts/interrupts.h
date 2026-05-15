/**
 * @file interrupts.h
 * @brief ISR mapping for STM32 dual-elevator system.
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "main/main.h"
#include "exti/exti.h"

/**
 * @brief Initializes all interrupt handlers.
 */
void Interrupts_Init(void);

#endif /* INTERRUPTS_H */
