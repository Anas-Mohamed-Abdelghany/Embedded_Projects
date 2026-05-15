/**
 * @file pwm.c
 * @brief PWM module implementation.
 */

#include "pwm.h"
#include "pwm_private.h"

void PWM_Init_Motor(void) {
    /* Set Prescaler and ARR */
    TIM3->PSC = TIM3_PRESCALER;
    TIM3->ARR = TIM3_PERIOD;
    
    /* Configure Channel 3 as PWM mode 1 */
    /* OC3M = 110 (PWM mode 1) */
    TIM3->CCMR2 &= ~TIM_CCMR2_OC3M;
    TIM3->CCMR2 |= (6U << TIM_CCMR2_OC3M_Pos);
    
    /* Enable Preload */
    TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
    
    /* Enable Output for Channel 3 */
    TIM3->CCER |= TIM_CCER_CC3E;
    
    /* Initialize with 0 duty cycle */
    TIM3->CCR3 = 0;
    
    /* Force update to load values */
    TIM3->EGR |= TIM_EGR_UG;
}

void PWM_Set_Duty(uint8_t duty) {
    if (duty > 100) duty = 100;
    
    /* Calculate compare value: duty * (ARR + 1) / 100 */
    TIM3->CCR3 = (uint32_t)((uint32_t)duty * (TIM3_PERIOD + 1)) / 100U;
}

void PWM_Start(void) {
    TIM3->CR1 |= TIM_CR1_CEN;
}

void PWM_Stop(void) {
    TIM3->CR1 &= ~TIM_CR1_CEN;
}
