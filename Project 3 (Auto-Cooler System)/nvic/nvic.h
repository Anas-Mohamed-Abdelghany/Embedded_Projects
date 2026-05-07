#ifndef NVIC_H
#define NVIC_H

#include <stdint.h>

/* Enables a specific interrupt in the NVIC. */
void Nvic_EnableIRQ(uint8_t irq_number);

/* Disables a specific interrupt in the NVIC. */
void Nvic_DisableIRQ(uint8_t irq_number);

/* Sets the priority for a specific interrupt. */
void Nvic_SetPriority(uint8_t irq_number, uint8_t priority);

#endif /* NVIC_H */
