/**
 * @file diagnostics.h
 * @brief Diagnostics and telemetry for STM32 dual-elevator system.
 */

#ifndef DIAGNOSTICS_H
#define DIAGNOSTICS_H

#include "main/main.h"
#include "exti/exti.h"
#include "state_machine/elevator_fsm.h"

/**
 * @brief Initializes the diagnostics module.
 */
void Diagnostics_Init(void);

/**
 * @brief Sends telemetry data over UART.
 * @param elev Pointer to local elevator object
 */
void Diagnostics_Send_Telemetry(elevator_t *elev);

#endif /* DIAGNOSTICS_H */
