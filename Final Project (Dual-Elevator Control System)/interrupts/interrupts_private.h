/**
 * @file interrupts_private.h
 * @brief Private definitions and NVIC register mapping.
 */

#ifndef INTERRUPTS_PRIVATE_H
#define INTERRUPTS_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"
#include "interrupts.h"

/* --- NVIC Register Structure --- */
typedef struct {
    volatile uint32_t ISER[8];       /* 0x000-0x01C */
    uint32_t Reserved0[24];
    volatile uint32_t ICER[8];       /* 0x080-0x09C */
    uint32_t Reserved1[24];
    volatile uint32_t ISPR[8];       /* 0x100-0x11C */
    uint32_t Reserved2[24];
    volatile uint32_t ICPR[8];       /* 0x180-0x19C */
    uint32_t Reserved3[24];
    volatile uint32_t IABR[8];       /* 0x200-0x21C */
    uint32_t Reserved4[56];
    volatile uint8_t  IPR[240];      /* 0x400-0x4EF */
} NVIC_Regs_t;

#define NVIC_BASE   (0xE000E100UL)
#define NVIC        ((NVIC_Regs_t *)NVIC_BASE)

/* --- IRQ Numbers --- */
typedef enum {
    IRQ_EXTI0 = 6,
    IRQ_EXTI1 = 7,
    IRQ_EXTI2 = 8,
    IRQ_EXTI3 = 9,
    IRQ_EXTI4 = 10,
    IRQ_EXTI9_5 = 23,
    IRQ_TIM2 = 28,
    IRQ_TIM3 = 29,
    IRQ_SPI1 = 35,
    IRQ_USART1 = 37,
    IRQ_EXTI15_10 = 40
} IRQn_Type;

/**
 * @brief Enables a specific interrupt in NVIC.
 */
static inline void NVIC_Enable_IRQ(IRQn_Type IRQn) {
    NVIC->ISER[(uint32_t)IRQn >> 5] = (1UL << ((uint32_t)IRQn & 0x1F));
}

/**
 * @brief Sets the priority of an interrupt.
 */
static inline void NVIC_Set_Priority(IRQn_Type IRQn, uint8_t priority) {
    NVIC->IPR[(uint32_t)IRQn] = (uint8_t)((priority << 4) & 0xFF);
}

/* Extern declarations for module handlers */
extern void Scheduler_Tick(void);
extern void SPI1_IRQHandler_Impl(void);
extern void USART1_IRQHandler_Impl(void);
extern void EXTI_Handler_Impl(uint8_t pin);

#endif /* INTERRUPTS_PRIVATE_H */
