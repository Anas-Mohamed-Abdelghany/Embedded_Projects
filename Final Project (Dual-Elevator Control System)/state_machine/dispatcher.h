/**
 * @file dispatcher.h
 * @brief Dispatcher for Master MCU to allocate hallway requests.
 */

#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "main/main.h"
#include "exti/exti.h"
#include "state_machine/elevator_fsm.h"

/**
 * @brief Initializes the dispatcher.
 */
void Dispatcher_Init(void);

/**
 * @brief Allocates a request to the best elevator based on mandated rules.
 */
void Dispatcher_Allocate_Request(uint8_t floor, direction_t dir, elevator_t *elev_a, elevator_t *elev_b, bool_t comm_fault);

#endif /* DISPATCHER_H */
