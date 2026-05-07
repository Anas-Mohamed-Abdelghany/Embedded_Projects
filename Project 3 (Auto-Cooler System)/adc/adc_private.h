/*
 * adc_private.h
 * ADC1 and NVIC register struct overlays — internal to adc module only.
 *
 * STM32F407 Reference Manual (RM0090):
 *   §13.8 — ADC Registers
 *   §13.3 — ADC functional description
 *
 * ARM Cortex-M4 Generic User Guide:
 *   §4.2 — NVIC Registers
 */

#ifndef ADC_PRIVATE_H
#define ADC_PRIVATE_H

#include <stdint.h>

/* ---- ADC1 Register Layout (RM0090 §13.8) ------------------------- */
typedef struct {
    volatile uint32_t SR;       /* 0x00 - Status Register              */
    volatile uint32_t CR1;      /* 0x04 - Control Register 1           */
    volatile uint32_t CR2;      /* 0x08 - Control Register 2           */
    volatile uint32_t SMPR1;    /* 0x0C - Sample Time Reg (ch 10-18)   */
    volatile uint32_t SMPR2;    /* 0x10 - Sample Time Reg (ch 0-9)     */
    volatile uint32_t JOFR1;    /* 0x14 - Injected Channel Offset 1    */
    volatile uint32_t JOFR2;    /* 0x18 */
    volatile uint32_t JOFR3;    /* 0x1C */
    volatile uint32_t JOFR4;    /* 0x20 */
    volatile uint32_t HTR;      /* 0x24 - Watchdog High Threshold       */
    volatile uint32_t LTR;      /* 0x28 - Watchdog Low Threshold        */
    volatile uint32_t SQR1;     /* 0x2C - Regular Sequence Register 1  */
    volatile uint32_t SQR2;     /* 0x30 - Regular Sequence Register 2  */
    volatile uint32_t SQR3;     /* 0x34 - Regular Sequence Register 3  */
    volatile uint32_t JSQR;     /* 0x38 - Injected Sequence Register   */
    volatile uint32_t JDR1;     /* 0x3C - Injected Data Register 1     */
    volatile uint32_t JDR2;     /* 0x40 */
    volatile uint32_t JDR3;     /* 0x44 */
    volatile uint32_t JDR4;     /* 0x48 */
    volatile uint32_t DR;       /* 0x4C - Regular Data Register        */
} AdcType;

/* ---- ADC Common Registers (shared across ADC1/2/3) --------------- */
typedef struct {
    volatile uint32_t CSR;  /* 0x00 - Common Status Register          */
    volatile uint32_t CCR;  /* 0x04 - Common Control Register         */
    volatile uint32_t CDR;  /* 0x08 - Common Regular Data Register    */
} AdcCommonType;

#define ADC1_BASE_ADDR        0x40012000UL
#define ADC_COMMON_BASE_ADDR  0x40012300UL

#define ADC1        ((AdcType *)       ADC1_BASE_ADDR)
#define ADC_COMMON  ((AdcCommonType *) ADC_COMMON_BASE_ADDR)

/* ---- ADC_SR bit positions ---------------------------------------- */
#define ADC_SR_EOC      (1UL << 1)    /* End of Conversion flag        */

/* ---- ADC_CR1 bit positions --------------------------------------- */
#define ADC_CR1_EOCIE   (1UL << 5)    /* EOC Interrupt Enable          */

/* ---- ADC_CR2 bit positions --------------------------------------- */
#define ADC_CR2_ADON    (1UL << 0)    /* ADC ON / OFF                  */
#define ADC_CR2_CONT    (1UL << 1)    /* Continuous Conversion Mode    */
#define ADC_CR2_SWSTART (1UL << 30)   /* Start Regular Conversions     */

/* ---- ADC_CCR bit positions (prescaler) --------------------------- */
/* ADCPRE [17:16] = 01 -> PCLK2 / 4 (RM0090 §13.8.1)                */
#define ADC_CCR_ADCPRE_DIV4   (1UL << 16)

/* ---- NVIC: ADC1/2/3 IRQ number (STM32F407 vector table) ---------- */
/* IRQ 18 = ADC (ADC1, ADC2, ADC3 global interrupt)                  */
#define ADC_IRQ_NUMBER  18U

#endif /* ADC_PRIVATE_H */
