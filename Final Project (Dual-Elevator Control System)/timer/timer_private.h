/**
 * @file timer_private.h
 * @brief Private definitions and register mapping for Timers.
 */

#ifndef TIMER_PRIVATE_H
#define TIMER_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"

/* --- TIM Register Structure --- */
typedef struct {
    volatile uint32_t CR1;           /* 0x00 */
    volatile uint32_t CR2;           /* 0x04 */
    volatile uint32_t SMCR;          /* 0x08 */
    volatile uint32_t DIER;          /* 0x0C */
    volatile uint32_t SR;            /* 0x10 */
    volatile uint32_t EGR;           /* 0x14 */
    volatile uint32_t CCMR1;         /* 0x18 */
    volatile uint32_t CCMR2;         /* 0x1C */
    volatile uint32_t CCER;          /* 0x20 */
    volatile uint32_t CNT;           /* 0x24 */
    volatile uint32_t PSC;           /* 0x28 */
    volatile uint32_t ARR;           /* 0x2C */
    uint32_t Reserved0;
    volatile uint32_t CCR1;          /* 0x34 */
    volatile uint32_t CCR2;          /* 0x38 */
    volatile uint32_t CCR3;          /* 0x3C */
    volatile uint32_t CCR4;          /* 0x40 */
    uint32_t Reserved1;
    volatile uint32_t DCR;           /* 0x48 */
    volatile uint32_t DMAR;          /* 0x4C */
    volatile uint32_t OR;            /* 0x50 */
} TIM_Regs_t;

#define TIM2_BASE   (0x40000000UL)
#define TIM2        ((TIM_Regs_t *)TIM2_BASE)

/* --- TIM Bitfields --- */
#define TIM_CR1_CEN       (1U << 0)
#define TIM_DIER_UIE      (1U << 0)
#define TIM_SR_UIF        (1U << 0)
#define TIM_EGR_UG        (1U << 0)

/* HSI = 16MHz, Target = 1ms tick (1000Hz) */
#define TIM2_PRESCALER    (16U - 1U)
#define TIM2_PERIOD       (1000U - 1U)

#endif /* TIMER_PRIVATE_H */
