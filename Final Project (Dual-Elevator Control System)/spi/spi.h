/**
 * @file spi.h
 * @brief SPI driver for Inter-Processor Communication (IPC).
 */

#ifndef SPI_H
#define SPI_H

#include "main/main.h"
#include "exti/exti.h"

#define SPI_PACKET_SIZE    8

/**
 * @brief Initializes SPI1 as Master or Slave.
 * @param is_master TRUE for Master, FALSE for Slave
 */
void SPI_Init_IPC(bool_t is_master);

/**
 * @brief Transmits and receives a packet over SPI (Non-blocking ISR based).
 * @param tx_buf Pointer to data to send
 * @param rx_buf Pointer to buffer for received data
 */
void SPI_Transfer_Async(uint8_t *tx_buf, uint8_t *rx_buf);

/**
 * @brief Checks if a transfer is in progress.
 * @return TRUE if busy
 */
bool_t SPI_Is_Busy(void);

#endif /* SPI_H */
