/**
 * @file timer.h
 * @brief Timer driver for scheduler tick (TIM2).
 */

#ifndef TIMER_H
#define TIMER_H

#include "main/main.h"
#include "exti/exti.h"

/**
 * @brief Initializes TIM2 for 1ms periodic interrupt.
 */
void Timer_Init_Scheduler(void);

/**
 * @brief Starts the scheduler timer.
 */
void Timer_Start(void);

/**
 * @brief Stops the scheduler timer.
 */
void Timer_Stop(void);

#endif /* TIMER_H */
