/**
 * @file rcc_private.h
 * @brief Private definitions and register mapping for RCC.
 */

#ifndef RCC_PRIVATE_H
#define RCC_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"

/* --- RCC Register Structure --- */
typedef struct {
    volatile uint32_t CR;            /* 0x00 */
    volatile uint32_t PLLCFGR;       /* 0x04 */
    volatile uint32_t CFGR;          /* 0x08 */
    volatile uint32_t CIR;           /* 0x0C */
    volatile uint32_t AHB1RSTR;      /* 0x10 */
    volatile uint32_t AHB2RSTR;      /* 0x14 */
    uint32_t Reserved0[2];
    volatile uint32_t APB1RSTR;      /* 0x20 */
    volatile uint32_t APB2RSTR;      /* 0x24 */
    uint32_t Reserved1[2];
    volatile uint32_t AHB1ENR;       /* 0x30 */
    volatile uint32_t AHB2ENR;       /* 0x34 */
    uint32_t Reserved2[2];
    volatile uint32_t APB1ENR;       /* 0x40 */
    volatile uint32_t APB2ENR;       /* 0x44 */
    uint32_t Reserved3[2];
    volatile uint32_t AHB1LPENR;     /* 0x50 */
    volatile uint32_t AHB2LPENR;     /* 0x54 */
    uint32_t Reserved4[2];
    volatile uint32_t APB1LPENR;     /* 0x60 */
    volatile uint32_t APB2LPENR;     /* 0x64 */
    uint32_t Reserved5[2];
    volatile uint32_t BDCR;          /* 0x70 */
    volatile uint32_t CSR;           /* 0x74 */
    uint32_t Reserved6[2];
    volatile uint32_t SSCGR;         /* 0x80 */
    volatile uint32_t PLLI2SCFGR;    /* 0x84 */
} RCC_Regs_t;

#define RCC_BASE    (0x40023800UL)
#define RCC         ((RCC_Regs_t *)RCC_BASE)

/* --- RCC Bitfields --- */
#define RCC_CR_HSION      (1U << 0)
#define RCC_CR_HSIRDY     (1U << 1)
#define RCC_CFGR_SW       (3U << 0)
#define RCC_CFGR_SW_HSI   (0U << 0)
#define RCC_CFGR_SWS      (3U << 2)
#define RCC_CFGR_SWS_HSI  (0U << 2)
#define RCC_CFGR_HPRE     (0xFU << 4)
#define RCC_CFGR_PPRE1    (7U << 10)
#define RCC_CFGR_PPRE2    (7U << 13)

/* Peripheral Enable Positions */
#define RCC_AHB1ENR_GPIOAEN_Pos    (0U)
#define RCC_AHB1ENR_GPIOBEN_Pos    (1U)
#define RCC_AHB1ENR_GPIOCEN_Pos    (2U)
#define RCC_AHB1ENR_GPIODEN_Pos    (3U)
#define RCC_AHB1ENR_GPIOEEN_Pos    (4U)

#define RCC_APB2ENR_SPI1EN_Pos     (12U)
#define RCC_APB2ENR_USART1EN_Pos   (4U)
#define RCC_APB2ENR_SYSCFGEN_Pos   (14U)

#define RCC_APB1ENR_TIM2EN_Pos     (0U)
#define RCC_APB1ENR_TIM3EN_Pos     (1U)

#endif /* RCC_PRIVATE_H */
