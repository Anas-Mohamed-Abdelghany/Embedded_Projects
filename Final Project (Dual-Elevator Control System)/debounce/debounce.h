/**
 * @file debounce.h
 * @brief Software debouncing for EXTI-triggered inputs.
 */

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "main/main.h"
#include "exti/exti.h"

#define DEBOUNCE_TIME_MS    50

/**
 * @brief Processes a raw input event and returns TRUE if debounced.
 * @param pin Pin index
 * @param raw_state Current raw state of the pin
 * @return TRUE if state changed and debounced
 */
bool_t Debounce_Process(uint8_t pin, uint8_t raw_state);

#endif /* DEBOUNCE_H */
