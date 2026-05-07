#ifndef NVIC_PRIVATE_H
#define NVIC_PRIVATE_H

#include <stdint.h>

/* NVIC Register Map (Cortex-M4 Generic User Guide §4.2) */
typedef struct {
    volatile uint32_t ISER[8];      /* 0x000 - Interrupt Set-Enable Registers      */
    uint32_t          RESERVED0[24];
    volatile uint32_t ICER[8];      /* 0x080 - Interrupt Clear-Enable Registers    */
    uint32_t          RESERVED1[24];
    volatile uint32_t ISPR[8];      /* 0x100 - Interrupt Set-Pending Registers     */
    uint32_t          RESERVED2[24];
    volatile uint32_t ICPR[8];      /* 0x180 - Interrupt Clear-Pending Registers   */
    uint32_t          RESERVED3[24];
    volatile uint32_t IABR[8];      /* 0x200 - Interrupt Active Bit Registers      */
    uint32_t          RESERVED4[56];
    volatile uint8_t  IP[240];       /* 0x400 - Interrupt Priority Registers (8-bit)*/
    uint32_t          RESERVED5[644];
    volatile uint32_t STIR;         /* 0xF00 - Software Trigger Interrupt Register */
} NvicType;

#define NVIC_BASE_ADDR   0xE000E100UL
#define NVIC             ((NvicType *) NVIC_BASE_ADDR)

#endif /* NVIC_PRIVATE_H */
