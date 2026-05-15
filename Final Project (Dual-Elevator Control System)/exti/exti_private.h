/**
 * @file exti_private.h
 * @brief Private definitions and register mapping for EXTI and SYSCFG.
 */

#ifndef EXTI_PRIVATE_H
#define EXTI_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"

/* --- EXTI Register Structure --- */
typedef struct {
    volatile uint32_t IMR;           /* 0x00 */
    volatile uint32_t EMR;           /* 0x04 */
    volatile uint32_t RTSR;          /* 0x08 */
    volatile uint32_t FTSR;          /* 0x0C */
    volatile uint32_t SWIER;         /* 0x10 */
    volatile uint32_t PR;            /* 0x14 */
} EXTI_Regs_t;

#define EXTI_BASE   (0x40013C00UL)
#define EXTI        ((EXTI_Regs_t *)EXTI_BASE)

/* --- SYSCFG Register Structure --- */
typedef struct {
    volatile uint32_t MEMRMP;        /* 0x00 */
    volatile uint32_t PMC;           /* 0x04 */
    volatile uint32_t EXTICR[4];     /* 0x08-0x14 */
    uint32_t Reserved[2];
    volatile uint32_t CMPCR;         /* 0x20 */
} SYSCFG_Regs_t;

#define SYSCFG_BASE (0x40013800UL)
#define SYSCFG      ((SYSCFG_Regs_t *)SYSCFG_BASE)

/* SYSCFG register positions */
#define SYSCFG_EXTICR_Pos(pin)    ((pin % 4U) * 4U)
#define SYSCFG_EXTICR_Index(pin)  (pin / 4U)

#endif /* EXTI_PRIVATE_H */
