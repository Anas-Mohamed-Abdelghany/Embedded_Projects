#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdint.h>

/* ─────────────── STATE DEFINITIONS ─────────────── */
typedef enum {
    STATE_LOCKED   = 0,
    STATE_UNLOCKED = 1,
    STATE_ALARM    = 2
} SystemState_t;

/* ─────────────── INPUT EVENT DEFINITIONS ─────────────── */
typedef enum {
    EVENT_NONE           = 0,
    EVENT_KEY_PRESSED,        
    EVENT_SEQUENCE_COMPLETE,
    EVENT_INVALID_KEY,      
    EVENT_LOCK_CMD,         /* Triggers from EXTI (Interrupt) */
    EVENT_DOORBELL,         /* Triggers from EXTI (Interrupt) */
    EVENT_EMERGENCY_RESET   /* Triggers from EXTI (Interrupt) */
} SystemEvent_t;

/* ─────────────── PUBLIC API ─────────────── */
void     SM_Init(void);

void     SM_ProcessEvent(SystemEvent_t event);

/* Use this to get a 'Local Copy' of the state safely. */
SystemState_t SM_GetState(void);

#endif