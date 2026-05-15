/**
 * @file uart.c
 * @brief UART module implementation.
 */

#include "uart.h"
#include "uart_private.h"
#include "interrupts/interrupts_private.h"

static uart_tx_fifo_t tx_fifo;

void UART_Init_Telemetry(void) {
    /* Set Baud Rate */
    USART1->BRR = UART1_BRR;
    
    /* Enable TX and RX */
    USART1->CR1 |= (USART_CR1_TE | USART_CR1_RE);
    
    /* Enable TXE interrupt */
    USART1->CR1 |= USART_CR1_TXEIE;
    
    /* NVIC Config */
    NVIC_Set_Priority(IRQ_USART1, 6); /* Lower priority for telemetry */
    NVIC_Enable_IRQ(IRQ_USART1);
    
    /* Enable USART */
    USART1->CR1 |= USART_CR1_UE;
    
    tx_fifo.head = 0;
    tx_fifo.tail = 0;
    tx_fifo.busy = FALSE;
}

void UART_Send_Data(const uint8_t *data, uint16_t len) {
    Enter_Critical();
    for (uint16_t i = 0; i < len; i++) {
        uint16_t next = (tx_fifo.head + 1) % TX_BUF_SIZE;
        if (next != tx_fifo.tail) {
            tx_fifo.buffer[tx_fifo.head] = data[i];
            tx_fifo.head = next;
        }
    }
    /* Ensure TXE interrupt is enabled to start transmission */
    USART1->CR1 |= USART_CR1_TXEIE;
    Exit_Critical();
}

void UART_Send_String(const char *str) {
    uint16_t len = 0;
    while (str[len] != '\0') len++;
    UART_Send_Data((const uint8_t *)str, len);
}

void UART_Send_String_Blocking(const char *str) {
    while (*str) {
        /* Wait until TXE (Transmit data register empty) is set */
        while (!(USART1->SR & USART_SR_TXE));
        /* Write character to DR */
        USART1->DR = (uint8_t)(*str++);
    }
    /* Wait for Transmission Complete */
    while (!(USART1->SR & (1U << 6))); /* TC bit */
}

/**
 * @brief USART1 Interrupt Handler (to be called from interrupts.c)
 */
void USART1_IRQHandler_Impl(void) {
    /* TXE (Transmit data register empty) */
    if ((USART1->SR & USART_SR_TXE) && (USART1->CR1 & USART_CR1_TXEIE)) {
        if (tx_fifo.head != tx_fifo.tail) {
            USART1->DR = tx_fifo.buffer[tx_fifo.tail];
            tx_fifo.tail = (tx_fifo.tail + 1) % TX_BUF_SIZE;
        } else {
            /* No more data, disable TXE interrupt */
            USART1->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
