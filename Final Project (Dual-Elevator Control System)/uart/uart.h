/**
 * @file uart.h
 * @brief UART driver for telemetry (USART1).
 */

#ifndef UART_H
#define UART_H

#include "main/main.h"
#include "exti/exti.h"

/**
 * @brief Initializes USART1 for telemetry.
 */
void UART_Init_Telemetry(void);

/**
 * @brief Sends a string over UART (Non-blocking DMA or Interrupt based).
 * @param str Null-terminated string
 */
void UART_Send_String(const char *str);

/**
 * @brief Sends a block of data over UART.
 * @param data Pointer to data
 * @param len Length in bytes
 */
void UART_Send_Data(const uint8_t *data, uint16_t len);
void UART_Send_String(const char *str);
void UART_Send_String_Blocking(const char *str);

#endif /* UART_H */
