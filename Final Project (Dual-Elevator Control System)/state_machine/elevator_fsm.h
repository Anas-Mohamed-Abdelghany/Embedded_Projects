/**
 * @file elevator_fsm.h
 * @brief Elevator Finite State Machine for STM32 dual-elevator system.
 */

#ifndef ELEVATOR_FSM_H
#define ELEVATOR_FSM_H

#include "main/main.h"
#include "queue/queue.h"

/* --- Elevator State Machine Types --- */
typedef enum {
    STATE_IDLE = 0,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_DOORS_OPENING,
    STATE_DOORS_OPEN,
    STATE_DOORS_CLOSING,
    STATE_EMERGENCY_STOP
} elevator_state_t;

/* Elevator Timing Constants (ms/ticks) */
#define DOOR_OPERATION_TIME    500
#define DOOR_OPEN_TIME         500
#define TRAVEL_TIME_PER_FLOOR  500

typedef struct {
    uint8_t id;
    volatile elevator_state_t state;
    volatile uint8_t current_floor;
    volatile uint8_t target_floor;
    volatile direction_t direction;
    request_queue_t queue;
    uint32_t state_timer;
} elevator_t;

/* --- Public Function Prototypes --- */
void Elevator_FSM_Init(elevator_t *elev, uint8_t id);
void Elevator_FSM_Update(elevator_t *elev);
void Elevator_Handle_Request(elevator_t *elev, uint8_t floor, direction_t dir);

#endif /* ELEVATOR_FSM_H */
