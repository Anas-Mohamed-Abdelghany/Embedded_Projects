/**
 * @file gpio_private.h
 * @brief Private definitions and register mapping for GPIO.
 */

#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"
#include "gpio.h"

/* --- GPIO Register Structure --- */
typedef struct {
    volatile uint32_t MODER;         /* 0x00 */
    volatile uint32_t OTYPER;        /* 0x04 */
    volatile uint32_t OSPEEDR;       /* 0x08 */
    volatile uint32_t PUPDR;         /* 0x0C */
    volatile uint32_t IDR;           /* 0x10 */
    volatile uint32_t ODR;           /* 0x14 */
    volatile uint32_t BSRR;          /* 0x18 */
    volatile uint32_t LCKR;          /* 0x1C */
    volatile uint32_t AFR[2];        /* 0x20-0x24 */
} GPIO_Regs_t;

#define GPIOA_BASE  (0x40020000UL)
#define GPIOB_BASE  (0x40020400UL)
#define GPIOC_BASE  (0x40020800UL)
#define GPIOD_BASE  (0x40020C00UL)
#define GPIOE_BASE  (0x40021000UL)

#define GPIOA       ((GPIO_Regs_t *)GPIOA_BASE)
#define GPIOB       ((GPIO_Regs_t *)GPIOB_BASE)
#define GPIOC       ((GPIO_Regs_t *)GPIOC_BASE)
#define GPIOD       ((GPIO_Regs_t *)GPIOD_BASE)
#define GPIOE       ((GPIO_Regs_t *)GPIOE_BASE)

/* Registers bitfield positions */
#define GPIO_MODER_Pos(pin)     (pin * 2U)
#define GPIO_PUPDR_Pos(pin)     (pin * 2U)
#define GPIO_OSPEEDR_Pos(pin)   (pin * 2U)
#define GPIO_OTYPER_Pos(pin)    (pin)
#define GPIO_AFR_Pos(pin)       ((pin % 8U) * 4U)

#endif /* GPIO_PRIVATE_H */
