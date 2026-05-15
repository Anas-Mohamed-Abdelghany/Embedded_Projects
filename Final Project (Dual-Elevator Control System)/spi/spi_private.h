/**
 * @file spi_private.h
 * @brief Private definitions and register mapping for SPI.
 */

#ifndef SPI_PRIVATE_H
#define SPI_PRIVATE_H

#include "main/main.h"
#include "exti/exti.h"
#include "spi.h"

/* --- SPI Register Structure --- */
typedef struct {
    volatile uint32_t CR1;           /* 0x00 */
    volatile uint32_t CR2;           /* 0x04 */
    volatile uint32_t SR;            /* 0x08 */
    volatile uint32_t DR;            /* 0x0C */
    volatile uint32_t CRCPR;         /* 0x10 */
    volatile uint32_t RXCRCR;        /* 0x14 */
    volatile uint32_t TXCRCR;        /* 0x18 */
    volatile uint32_t I2SCFGR;       /* 0x1C */
    volatile uint32_t I2SPR;         /* 0x20 */
} SPI_Regs_t;

#define SPI1_BASE   (0x40013000UL)
#define SPI1        ((SPI_Regs_t *)SPI1_BASE)

/* --- SPI Bitfields --- */
#define SPI_CR1_SPE       (1U << 6)
#define SPI_CR1_MSTR      (1U << 2)
#define SPI_CR1_BR_Pos    3
#define SPI_CR1_CPOL      (1U << 1)
#define SPI_CR1_CPHA      (1U << 0)
#define SPI_CR1_DFF       (1U << 11)
#define SPI_CR1_LSBFIRST  (1U << 7)
#define SPI_CR1_SSM       (1U << 9)
#define SPI_CR1_SSI       (1U << 8)

#define SPI_CR2_TXEIE     (1U << 7)
#define SPI_CR2_RXNEIE    (1U << 6)

#define SPI_SR_TXE        (1U << 1)
#define SPI_SR_RXNE       (1U << 0)

typedef struct {
    uint8_t *tx_ptr;
    uint8_t *rx_ptr;
    volatile uint8_t tx_count;
    volatile uint8_t rx_count;
    volatile bool_t busy;
} spi_handle_t;

#endif /* SPI_PRIVATE_H */
