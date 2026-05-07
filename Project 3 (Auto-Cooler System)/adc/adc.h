/*
 * Mode     : Continuous conversion, 12-bit resolution.
 * Readout  : End-of-Conversion (EOC) interrupt — no CPU polling.
 * Callback : Fires on every completed conversion.
 *
 * LM35 transfer: Vout = 10 mV/°C
 *   Vref = 3.3 V, 12-bit ADC (4096 steps)
 *   Temp(°C) = raw × 3.3 × 100 / 4095
 *            = raw × 330 / 4095
 *
 * Hardware: PA1 → ADC1_IN1 (configure GPIO as Analog before calling Adc_Init)
 */

#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* Callback type: called from ISR context on every new ADC reading */
typedef void (*AdcCallback_t)(uint16_t raw_value);

void  Adc_Init(AdcCallback_t callback);
void  Adc_StartConversion(void);
float Adc_RawToTemperature(uint16_t raw);

#endif /* ADC_H */
