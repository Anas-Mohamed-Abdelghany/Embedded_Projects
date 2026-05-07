/*
 * PWM Configuration and Alternate Function:
 *   1. Configure GPIO pin as Alternate Function, push-pull.
 *   2. Set the AF number in AFR (AF2 = TIM3 for PB4, per datasheet).
 *   3. Configure TIM3 time-base (PSC, ARR).
 *   4. Set CCMR1 for PWM Mode 1 with preload enable.
 *   5. Enable channel output in CCER.
 *   6. Force shadow-register load via EGR UG bit.
 *   7. Start the counter (CR1 CEN).
 *
 * CCR1 formula:  CCR1 = duty% × (ARR + 1) / 100
 */

#include "pwm.h"
#include "pwm_private.h"
#include "gpio.h"

/* ================================================================== */
/* Pwm_Init                                                             */
/* ================================================================== */
void Pwm_Init(void)
{
    /* -- 1. PB4: Alternate Function mode, push-pull output ----------- */
    /* Alternate Function: Routing the Timer to a Pin*/
    Gpio_Init(GPIO_B, 4, GPIO_AF, GPIO_PUSH_PULL);

    /* -- 2. Select AF2 (TIM3_CH1) on PB4 ---------------------------- */
    /* AF2 = TIM3 for PB4 (STM32F407 datasheet, Alternate Function table) */
    Gpio_SetAF(GPIO_B, 4, 2);

    /* -- 3. TIM3 time-base ------------------------------------------ */
    TIM3->CR1 = 0;              /* reset control register              */
    TIM3->PSC = PWM_PSC;        /* PSC=15 → FCNT = 16MHz/16 = 1 MHz   */
    TIM3->ARR = PWM_ARR;        /* ARR=999 → f = 1MHz/1000 = 1 kHz    */
    TIM3->CNT = 0;

    /* -- 4. CCMR1: CH1 PWM Mode 1, preload enable ------------------- */
    /* OC1M [6:4] = 110 → PWM Mode 1 (HIGH while CNT < CCR1)          */
    /* OC1PE [3]  = 1   → preload: CCR1 update takes effect at UEV    */
    TIM3->CCMR1 = TIM_CCMR1_OC1M_PWM1 | TIM_CCMR1_OC1PE;

    /* -- 5. CCER: enable CH1 output (CC1E bit) ----------------------- */
    TIM3->CCER = TIM_CCER_CC1E;

    /* -- 6. Initial duty cycle = 0% (fan OFF) ------------------------ */
    TIM3->CCR1 = 0;

    /* -- 7. Auto-reload preload + force shadow registers to load now - */
    /* Without this the timer starts with stale shadow values (§6.2) */
    TIM3->CR1 |= TIM_CR1_ARPE;
    TIM3->EGR  = TIM_EGR_UG;   /* force immediate update event         */

    /* -- 8. Start counter ------------------------------------------- */
    TIM3->CR1 |= TIM_CR1_CEN;
}

/* ================================================================== */
/* Pwm_SetDuty                                                          */
/* Updates CCR1 to change the fan duty cycle on the fly.              */
/* (Tutorial 7 §12.4 — Duty = CCR / (ARR+1) × 100%)                  */
/* ================================================================== */
void Pwm_SetDuty(uint8_t percent)
{
    if (percent > 100U)
    {
        percent = 100U;
    }

    /* CCR1 = duty% × (ARR + 1) / 100 */
    TIM3->CCR1 = ((uint32_t)percent * (PWM_ARR + 1UL)) / 100UL;
}
