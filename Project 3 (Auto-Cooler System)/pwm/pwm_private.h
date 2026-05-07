/*
 * STM32F407 Reference Manual (RM0090) §18 — General-purpose timers (TIMx)
 * TIM3 base address: 0x40000400
 *
 * PWM calculation:
 *   FCNT  = Fclk / (PSC + 1)  =  16 MHz / (15+1)  = 1 MHz
 *   T_pwm = (ARR + 1) / FCNT  =  1000 / 1 MHz      = 1 ms  → f = 1 kHz
 *   Duty  = CCR1 / (ARR + 1) × 100 %
 */

#ifndef PWM_PRIVATE_H
#define PWM_PRIVATE_H

#include <stdint.h>

/* Struct mirrors TIM3 register layout (offsets from RM0090 §18.4) */
typedef struct {
    volatile uint32_t CR1;      /* 0x00 - Control Register 1             */
    volatile uint32_t CR2;      /* 0x04 - Control Register 2             */
    volatile uint32_t SMCR;     /* 0x08 - Slave Mode Control Register    */
    volatile uint32_t DIER;     /* 0x0C - DMA/Interrupt Enable Register  */
    volatile uint32_t SR;       /* 0x10 - Status Register                */
    volatile uint32_t EGR;      /* 0x14 - Event Generation Register      */
    volatile uint32_t CCMR1;    /* 0x18 - Capture/Compare Mode Reg 1     */
    volatile uint32_t CCMR2;    /* 0x1C - Capture/Compare Mode Reg 2     */
    volatile uint32_t CCER;     /* 0x20 - Capture/Compare Enable Reg     */
    volatile uint32_t CNT;      /* 0x24 - Counter Register               */
    volatile uint32_t PSC;      /* 0x28 - Prescaler Register             */
    volatile uint32_t ARR;      /* 0x2C - Auto-Reload Register           */
    volatile uint32_t RESERVED; /* 0x30 - (reserved on general timers)   */
    volatile uint32_t CCR1;     /* 0x34 - Capture/Compare Register 1     */
    volatile uint32_t CCR2;     /* 0x38 - Capture/Compare Register 2     */
    volatile uint32_t CCR3;     /* 0x3C - Capture/Compare Register 3     */
    volatile uint32_t CCR4;     /* 0x40 - Capture/Compare Register 4     */
} TimerType;

#define TIM3_BASE_ADDR   0x40000400UL
#define TIM3             ((TimerType *) TIM3_BASE_ADDR)

/* CR1 bit positions (RM0090 §18.4.1) */
#define TIM_CR1_CEN    (1UL << 0)   /* Counter Enable                   */
#define TIM_CR1_ARPE   (1UL << 7)   /* Auto-Reload Preload Enable       */

/* EGR bit positions (RM0090 §18.4.6) */
#define TIM_EGR_UG     (1UL << 0)   /* Update Generation (shadow reload)*/

/* CCMR1 — Channel 1 PWM Mode 1: OC1M[6:4]=110, OC1PE[3]=1 */
/* OC1M = 110 (PWM Mode 1): HIGH while CNT < CCR1 (Tutorial 7 §12.3) */
#define TIM_CCMR1_OC1M_PWM1   (6UL << 4)   /* bits [6:4] = 110         */
#define TIM_CCMR1_OC1PE       (1UL << 3)   /* Output Compare 1 Preload */

/* CCER bit positions (RM0090 §18.4.9) */
#define TIM_CCER_CC1E  (1UL << 0)   /* Capture/Compare Channel 1 Enable */

/* PWM timing constants for 1 kHz at 16 MHz HSI:
 *   PSC = 15  →  FCNT = 16 MHz / 16 = 1 MHz
 *   ARR = 999 →  f    = 1 MHz / 1000 = 1 kHz                         */
/* Change these values in pwm_private.h */
#define PWM_PSC    53U     // Adjusted for Proteus (159 / 3)
#define PWM_ARR    999U    // 100kHz / 1000 = 100 Hz Frequency

#endif /* PWM_PRIVATE_H */
