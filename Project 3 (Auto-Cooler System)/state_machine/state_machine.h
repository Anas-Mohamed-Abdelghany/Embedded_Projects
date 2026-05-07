/*
 * state_machine.h
 * Public API for the Mealy State Machine controlling the Auto-Cooler.
 *
 * In a Mealy machine every output depends on BOTH the current state
 * AND the current input. Here:
 *   Input  = temperature reading from ADC
 *   States = IDLE, COOLING, OVERHEAT
 *   Outputs = PWM duty cycle + LCD content + Alarm LED level
 *
 * Temperature thresholds (project spec):
 *   T < 25°C       → 0%   fan  (IDLE)
 *   25 ≤ T < 30°C  → 33%  fan  (COOLING)
 *   30 ≤ T < 35°C  → 66%  fan  (COOLING)
 *   T ≥ 35°C       → 100% fan  (COOLING)
 *   T ≥ 40°C       → 100% fan + alarm LED + OVERHEAT on LCD (OVERHEAT)
 */

#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

typedef enum {
    STATE_IDLE     = 0,
    STATE_COOLING  = 1,
    STATE_OVERHEAT = 2
} SystemState;

void        FSM_Init   (void);
void        FSM_Update (float temperature);
SystemState FSM_GetState(void);

#endif /* STATE_MACHINE_H */
