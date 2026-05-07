#ifndef MAIN_PRIVATE_H_
#define MAIN_PRIVATE_H_

#include <stdint.h>

/* System Control Block - FPU Access Control Register */
#define SCB_CPACR    (* (volatile uint32_t *) 0xE000ED88UL)

/* Callback for ADC conversion complete. */
static void on_adc_reading(uint16_t raw);

#endif /* MAIN_PRIVATE_H_ */
