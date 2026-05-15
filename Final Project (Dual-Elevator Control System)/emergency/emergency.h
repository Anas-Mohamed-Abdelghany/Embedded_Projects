/**
 * @file emergency.h
 * @brief Emergency handling for STM32 dual-elevator system.
 */

#ifndef EMERGENCY_H
#define EMERGENCY_H

#include "main/main.h"
#include "exti/exti.h"

typedef enum {
    EMERGENCY_NONE = 0,
    EMERGENCY_STOP_PRESSED,
    EMERGENCY_COMM_FAULT,
    EMERGENCY_SENSOR_FAULT
} emergency_type_t;

/**
 * @brief Initializes the emergency module.
 */
void Emergency_Init(void);

/**
 * @brief Triggers an emergency state.
 * @param type Type of emergency
 */
void Emergency_Trigger(emergency_type_t type);

/**
 * @brief Checks if the system is in an emergency state.
 */
bool_t Emergency_Is_Active(void);

/**
 * @brief Resets the emergency state if safe.
 */
void Emergency_Reset(void);

#endif /* EMERGENCY_H */
