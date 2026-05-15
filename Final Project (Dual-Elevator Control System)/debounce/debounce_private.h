/**
 * @file debounce_private.h
 * @brief Private definitions for Debounce module.
 */

#ifndef DEBOUNCE_PRIVATE_H
#define DEBOUNCE_PRIVATE_H

#include "debounce.h"

#define NUM_PINS    16

typedef struct {
    uint8_t last_state;
    uint32_t last_tick;
} debounce_state_t;

#endif /* DEBOUNCE_PRIVATE_H */
