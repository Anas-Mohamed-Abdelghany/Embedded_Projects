/**
 * @file timer.c
 * @brief Timer module implementation.
 */

#include "timer.h"
#include "timer_private.h"
#include "interrupts/interrupts_private.h"

void Timer_Init_Scheduler(void) {
    /* Set Prescaler */
    TIM2->PSC = TIM2_PRESCALER;
    
    /* Set Auto-reload value */
    TIM2->ARR = TIM2_PERIOD;
    
    /* Enable Update Interrupt */
    TIM2->DIER |= TIM_DIER_UIE;
    
    /* Enable TIM2 IRQ in NVIC */
    NVIC_Set_Priority(IRQ_TIM2, 5); /* Medium priority for scheduler */
    NVIC_Enable_IRQ(IRQ_TIM2);
}

void Timer_Start(void) {
    TIM2->CR1 |= TIM_CR1_CEN;
}

void Timer_Stop(void) {
    TIM2->CR1 &= ~TIM_CR1_CEN;
}
