/*
 * rcc_private.h
 * RCC register struct overlay — internal to rcc module only.
 * The application never includes this file.
 *
 * STM32F407 Reference Manual (RM0090) §6.3
 * RCC base address: 0x40023800
 */

#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include <stdint.h>

/* Struct mirrors the RCC register layout in memory (offsets from RM0090) */
typedef struct {
    volatile uint32_t CR;           /* 0x00 - Clock Control Register        */
    volatile uint32_t PLLCFGR;      /* 0x04 - PLL Configuration             */
    volatile uint32_t CFGR;         /* 0x08 - Clock Configuration           */
    volatile uint32_t CIR;          /* 0x0C - Clock Interrupt Register       */
    volatile uint32_t AHB1RSTR;     /* 0x10 */
    volatile uint32_t AHB2RSTR;     /* 0x14 */
    volatile uint32_t AHB3RSTR;     /* 0x18 */
    volatile uint32_t RESERVED0;    /* 0x1C */
    volatile uint32_t APB1RSTR;     /* 0x20 */
    volatile uint32_t APB2RSTR;     /* 0x24 */
    volatile uint32_t RESERVED1;    /* 0x28 */
    volatile uint32_t RESERVED2;    /* 0x2C */
    volatile uint32_t AHB1ENR;      /* 0x30 - AHB1 Peripheral Clock Enable  */
    volatile uint32_t AHB2ENR;      /* 0x34 */
    volatile uint32_t AHB3ENR;      /* 0x38 */
    volatile uint32_t RESERVED3;    /* 0x3C */
    volatile uint32_t APB1ENR;      /* 0x40 - APB1 Peripheral Clock Enable  */
    volatile uint32_t APB2ENR;      /* 0x44 - APB2 Peripheral Clock Enable  */
} RccType;

#define RCC_BASE_ADDR   0x40023800UL
#define RCC             ((RccType *) RCC_BASE_ADDR)

#endif /* RCC_PRIVATE_H */
