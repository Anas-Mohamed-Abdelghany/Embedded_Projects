#ifndef GPIO_PRIVATE_H
#define GPIO_PRIVATE_H

#include <stdint.h>

/* Struct mirrors the GPIO register layout */
typedef struct {
    volatile uint32_t MODER;    /* 0x00 - Mode Register (2 bits per pin) */
    volatile uint32_t OTYPER;   /* 0x04 - Output Type Register           */
    volatile uint32_t OSPEEDR;  /* 0x08 - Output Speed Register          */
    volatile uint32_t PUPDR;    /* 0x0C - Pull-Up/Down Register          */
    volatile uint32_t IDR;      /* 0x10 - Input Data Register (read-only)*/
    volatile uint32_t ODR;      /* 0x14 - Output Data Register           */
    volatile uint32_t BSRR;     /* 0x18 - Bit Set/Reset Register         */
    volatile uint32_t LCKR;     /* 0x1C - Configuration Lock Register    */
    volatile uint32_t AFR[2];   /* 0x20 - Alternate Function Registers   */
} GpioType;

/* Base addresses */
#define GPIOA_BASE_ADDR   0x40020000UL
#define GPIOB_BASE_ADDR   0x40020400UL
#define GPIOC_BASE_ADDR   0x40020800UL
#define GPIOD_BASE_ADDR   0x40020C00UL

/* Cast base addresses to struct pointers — zero RAM overhead */
#define GPIOA   ((GpioType *) GPIOA_BASE_ADDR)
#define GPIOB   ((GpioType *) GPIOB_BASE_ADDR)
#define GPIOC   ((GpioType *) GPIOC_BASE_ADDR)
#define GPIOD   ((GpioType *) GPIOD_BASE_ADDR)

/* Lookup table: addressMap['X' - 'A'] -> base address of port X */
static const uint32_t addressMap[4] = {
    GPIOA_BASE_ADDR,    /* index 0 -> Port A */
    GPIOB_BASE_ADDR,    /* index 1 -> Port B */
    GPIOC_BASE_ADDR,    /* index 2 -> Port C */
    GPIOD_BASE_ADDR     /* index 3 -> Port D */
};

#endif /* GPIO_PRIVATE_H */
