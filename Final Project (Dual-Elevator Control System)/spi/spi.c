/**
 * @file spi.c
 * @brief SPI module implementation.
 */

#include "spi.h"
#include "spi_private.h"
#include "interrupts/interrupts_private.h"

static spi_handle_t hspi1;

void SPI_Init_IPC(bool_t is_master) {
    /* Disable SPI before config */
    SPI1->CR1 &= ~SPI_CR1_SPE;
    
    /* 1. Set Baud Rate (PCLK2=16MHz, BR=f/16 = 1MHz) */
    SPI1->CR1 |= (3U << SPI_CR1_BR_Pos);
    
    /* 2. CPOL=0, CPHA=0 (Mode 0) */
    SPI1->CR1 &= ~(SPI_CR1_CPOL | SPI_CR1_CPHA);
    
    /* 3. 8-bit Data Frame */
    SPI1->CR1 &= ~SPI_CR1_DFF;
    
    /* 4. MSB First */
    SPI1->CR1 &= ~SPI_CR1_LSBFIRST;
    
    /* 5. Master/Slave Mode */
    if (is_master) {
        SPI1->CR1 |= SPI_CR1_MSTR;
        /* Software Slave Management for Master */
        SPI1->CR1 |= (SPI_CR1_SSM | SPI_CR1_SSI);
    } else {
        SPI1->CR1 &= ~SPI_CR1_MSTR;
        /* Hardware Slave Management for Slave */
        SPI1->CR1 &= ~SPI_CR1_SSM;
    }
    
    /* 6. Enable Interrupts */
    SPI1->CR2 |= (SPI_CR2_TXEIE | SPI_CR2_RXNEIE);
    
    /* NVIC Config */
    NVIC_Set_Priority(IRQ_SPI1, 2); /* High priority for IPC */
    NVIC_Enable_IRQ(IRQ_SPI1);
    
    /* Enable SPI */
    SPI1->CR1 |= SPI_CR1_SPE;
    
    hspi1.busy = FALSE;
}

void SPI_Transfer_Async(uint8_t *tx_buf, uint8_t *rx_buf) {
    Enter_Critical();
    hspi1.tx_ptr = tx_buf;
    hspi1.rx_ptr = rx_buf;
    hspi1.tx_count = 0;
    hspi1.rx_count = 0;
    hspi1.busy = TRUE;
    
    /* Slave Challenge: Pre-load first byte if Slave */
    if (!(SPI1->CR1 & SPI_CR1_MSTR)) {
        SPI1->DR = hspi1.tx_ptr[0];
    }
    Exit_Critical();
}

bool_t SPI_Is_Busy(void) {
    return hspi1.busy;
}

/**
 * @brief SPI1 Interrupt Handler (to be called from interrupts.c)
 */
void SPI1_IRQHandler_Impl(void) {
    /* RXNE (Receive buffer not empty) */
    if (SPI1->SR & SPI_SR_RXNE) {
        uint8_t data = (uint8_t)SPI1->DR;
        if (hspi1.busy && hspi1.rx_ptr && hspi1.rx_count < SPI_PACKET_SIZE) {
            hspi1.rx_ptr[hspi1.rx_count] = data;
        }
        hspi1.rx_count++;
        
        if (hspi1.rx_count >= SPI_PACKET_SIZE) {
            hspi1.busy = FALSE;
        }
    }
    
    /* TXE (Transmit buffer empty) */
    if (SPI1->SR & SPI_SR_TXE) {
        if (hspi1.busy && hspi1.tx_count < SPI_PACKET_SIZE) {
            /* If Slave, first byte is already loaded manually */
            if (!(SPI1->CR1 & SPI_CR1_MSTR) && hspi1.tx_count == 0) {
                hspi1.tx_count++;
            }
            
            if (hspi1.tx_count < SPI_PACKET_SIZE) {
                SPI1->DR = hspi1.tx_ptr[hspi1.tx_count];
                hspi1.tx_count++;
            }
        }
    }
}
