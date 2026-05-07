#include "nvic.h"
#include "nvic_private.h"

void Nvic_EnableIRQ(uint8_t irq_number)
{
    uint8_t reg_idx = irq_number / 32;
    uint8_t bit_pos = irq_number % 32;
    
    if (reg_idx < 8) {
        NVIC->ISER[reg_idx] = (1UL << bit_pos);
    }
}

void Nvic_DisableIRQ(uint8_t irq_number)
{
    uint8_t reg_idx = irq_number / 32;
    uint8_t bit_pos = irq_number % 32;
    
    if (reg_idx < 8) {
        NVIC->ICER[reg_idx] = (1UL << bit_pos);
    }
}

void Nvic_SetPriority(uint8_t irq_number, uint8_t priority)
{
    /* STM32F4 uses 4 bits for priority (upper 4 bits of the 8-bit register) */
    if (irq_number < 240) {
        NVIC->IP[irq_number] = (uint8_t)((priority << 4) & 0xFFU);
    }
}
