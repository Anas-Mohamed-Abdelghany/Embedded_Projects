/*
 * Encodes bus selection and bit position from the peripheral ID,
 * then enables the correct bit in the correct ENR register.
 */

#include "rcc.h"
#include "rcc_private.h"

/* ------------------------------------------------------------------ */
/* Rcc_Init                                                             */
/* No PLL configuration — system runs on HSI at 16 MHz (default).     */
/* ------------------------------------------------------------------ */
void Rcc_Init(void)
{  }

/* ------------------------------------------------------------------ */
/* Rcc_Enable                                                           */
/* Decodes the peripheral ID and sets the correct bit in AHB1ENR, APB1ENR, or APB2ENR */
/* ------------------------------------------------------------------ */
void Rcc_Enable(uint16_t peripheral)
{
    uint8_t bus = (uint8_t)(peripheral >> 8);    /* upper byte = bus  */
    uint8_t bit = (uint8_t)(peripheral & 0x00FF); /* lower byte = bit  */

    switch (bus)
    {
        case 0x00:                              /* AHB1 bus */
            RCC->AHB1ENR |= (1UL << bit);
            break;

        case 0x01:                              /* APB1 bus */
            RCC->APB1ENR |= (1UL << bit);
            break;

        case 0x02:                              /* APB2 bus */
            RCC->APB2ENR |= (1UL << bit);
            break;

        default:
            /* Unknown bus — do nothing */
            break;
    }
}
