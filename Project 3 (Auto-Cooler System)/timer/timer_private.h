#ifndef TIMER_PRIVATE_H
#define TIMER_PRIVATE_H

#include <stdint.h>

/* TIM2 Register Map (RM0090 §18.4) */
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
    uint32_t reserved1;         /* 0x30 */
    volatile uint32_t CCR1;     /* 0x34 - Capture/Compare Register 1     */
    volatile uint32_t CCR2;     /* 0x38 - Capture/Compare Register 2     */
    volatile uint32_t CCR3;     /* 0x3C - Capture/Compare Register 3     */
    volatile uint32_t CCR4;     /* 0x40 - Capture/Compare Register 4     */
    uint32_t reserved2;         /* 0x44 */
    volatile uint32_t DCR;      /* 0x48 - DMA Control Register           */
    volatile uint32_t DMAR;     /* 0x4C - DMA Address for Full Transfer  */
    volatile uint32_t OR;       /* 0x50 - Option Register                */
} TimType;

#define TIM2_BASE_ADDR   0x40000000UL
#define TIM2             ((TimType *) TIM2_BASE_ADDR)

/* TIM_CR1 bit positions */
#define TIM_CR1_CEN      (1UL << 0)   /* Counter Enable */

/* TIM_DIER bit positions */
#define TIM_DIER_UIE     (1UL << 0)   /* Update Interrupt Enable */

/* TIM_SR bit positions */
#define TIM_SR_UIF       (1UL << 0)   /* Update Interrupt Flag */

#endif /* TIMER_PRIVATE_H */
