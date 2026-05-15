/**
 * @file debounce.c
 * @brief Debounce module implementation.
 */

#include "debounce.h"
#include "debounce_private.h"

static debounce_state_t pin_states[NUM_PINS];
extern volatile uint32_t system_tick; /* Assuming visible from scheduler.c or via a getter */

bool_t Debounce_Process(uint8_t pin, uint8_t raw_state) {
    if (pin >= NUM_PINS) return FALSE;
    
    /* Simple debounce: ignore changes within DEBOUNCE_TIME_MS */
    /* Note: In a real system, we might need a more complex state machine */
    if (raw_state != pin_states[pin].last_state) {
        // ... (simplified for now)
        pin_states[pin].last_state = raw_state;
        return TRUE;
    }
    
    return FALSE;
}
