/**
 * @file pwm.h
 * @brief PWM driver for motor simulation (TIM3).
 */

#ifndef PWM_H
#define PWM_H

#include "main/main.h"
#include "exti/exti.h"

/**
 * @brief Initializes TIM3 for PWM on CH3.
 */
void PWM_Init_Motor(void);

/**
 * @brief Sets the duty cycle of the motor PWM.
 * @param duty Percentage (0-100)
 */
void PWM_Set_Duty(uint8_t duty);

/**
 * @brief Starts the PWM output.
 */
void PWM_Start(void);
void PWM_Stop(void);

#endif /* PWM_H */
