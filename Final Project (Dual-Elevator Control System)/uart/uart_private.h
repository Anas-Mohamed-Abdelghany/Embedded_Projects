/**
 * @file uart_private.h
 * @brief Private definitions and register mapping for UART.
 */

#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"
#include "uart.h"

/* --- USART Register Structure --- */
typedef struct {
    volatile uint32_t SR;            /* 0x00 */
    volatile uint32_t DR;            /* 0x04 */
    volatile uint32_t BRR;           /* 0x08 */
    volatile uint32_t CR1;           /* 0x0C */
    volatile uint32_t CR2;           /* 0x10 */
    volatile uint32_t CR3;           /* 0x14 */
    volatile uint32_t GTPR;          /* 0x18 */
} USART_Regs_t;

#define USART1_BASE (0x40011000UL)
#define USART1      ((USART_Regs_t *)USART1_BASE)

/* --- USART Bitfields --- */
#define USART_CR1_UE      (1U << 13)
#define USART_CR1_TE      (1U << 3)
#define USART_CR1_RE      (1U << 2)
#define USART_CR1_TXEIE   (1U << 7)
#define USART_SR_TXE      (1U << 7)
#define USART_SR_RXNE     (1U << 5)

/* HSI = 16MHz, Baud = 115200 */
#define UART1_BRR    0x008B

#define TX_BUF_SIZE  256

typedef struct {
    uint8_t buffer[TX_BUF_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
    volatile bool_t busy;
} uart_tx_fifo_t;

#endif /* UART_PRIVATE_H */
