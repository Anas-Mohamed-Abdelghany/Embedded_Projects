#include "timer.h"
#include "timer_private.h"
#include "nvic.h"
#include <stdint.h>

static volatile uint32_t tick_ms = 0;

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        tick_ms++;
    }
}

void Timer_Init(void)
{
    /* TIM2 clock enable is handled in main.c via Rcc_Enable(RCC_TIM2) */
    
    /* PSC = 16-1 (1 MHz timer clock) */
    TIM2->PSC = 15U;
    
    /* ARR = 1000 (for exactly 1ms at 16MHz clock with PSC=16) */
    /* 1,000,000 / 1000 = 1,000 Hz */
    TIM2->ARR = 999;
    
    /* Force update to load PSC and ARR into shadow registers */
    TIM2->EGR |= 0x01; 
    /* Clear the UIF flag that the EGR just set so we don't fire an IRQ immediately */
    TIM2->SR &= ~TIM_SR_UIF;

    TIM2->DIER |= TIM_DIER_UIE;
    TIM2->CR1  |= TIM_CR1_CEN;

    /* Enable TIM2 IRQ in NVIC (IRQ 28) */
    Nvic_EnableIRQ(28);
}

void delay_ms(uint32_t ms)
{
    uint32_t start = tick_ms;
    while ((tick_ms - start) < ms)
    {
        /* Wait For Interrupt: Tells Proteus the CPU is idle.
           This significantly speeds up simulation by skipping cycles. */
        __asm volatile ("wfi");
    }
}

void delay_us(uint32_t us)
{
    volatile uint32_t count = us * 4; 
    while (count--);
}
