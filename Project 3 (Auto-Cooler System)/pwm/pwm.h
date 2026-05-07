/*
 * pwm.h
 * Public PWM driver API — TIM3 CH1 on PB4 (Alternate Function 2).
 *
 * Hardware : PB4 → TIM3_CH1 (AF2, STM32F407 datasheet alternate function table)
 * Frequency: 1 kHz (suitable for DC fan control, Tutorial 7 §12.2)
 * Duty     : 0%–100%, set via Pwm_SetDuty()
 *
 * The fan speed mapping (from project spec):
 *   T < 25°C       →  0%   (OFF)
 *   25 ≤ T < 30°C  → 33%
 *   30 ≤ T < 35°C  → 66%
 *   T ≥ 35°C       → 100% (full speed)
 */

#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void Pwm_Init(void);
void Pwm_SetDuty(uint8_t percent);   /* 0 – 100 */

#endif /* PWM_H */
