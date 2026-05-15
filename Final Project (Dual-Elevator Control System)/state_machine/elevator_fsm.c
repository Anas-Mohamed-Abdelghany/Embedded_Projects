/**
 * @file elevator_fsm.c
 * @brief Elevator Finite State Machine implementation.
 */

#include "elevator_fsm.h"
#include "elevator_fsm_private.h"
#include "pwm/pwm.h"
#include "gpio/gpio.h"
#include "main/pin_mapping.h"
#include "emergency/emergency.h"

void Elevator_FSM_Init(elevator_t *elev, uint8_t id) {
    elev->id = id;
    elev->state = STATE_IDLE;
    elev->current_floor = 1;
    elev->target_floor = 1;
    elev->direction = DIR_NONE;
    elev->state_timer = 0;
    Queue_Init(&elev->queue);
}

static void Update_Motor_Speed(elevator_t *elev) {
    switch(elev->state) {
        case STATE_MOVING_UP:
            GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN1_PIN, TRUE);
            GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN2_PIN, FALSE);
            PWM_Set_Duty(SPEED_FULL);
            break;
        case STATE_MOVING_DOWN:
            GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN1_PIN, FALSE);
            GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN2_PIN, TRUE);
            PWM_Set_Duty(SPEED_FULL);
            break;
        default:
            GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN1_PIN, FALSE);
            GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN2_PIN, FALSE);
            PWM_Set_Duty(SPEED_STOP);
            break;
    }
}

/* Checks if elevator should stop at the current floor (intermediate or final) */
static bool_t Should_Stop_At(elevator_t *elev) {
    /* 1. Final destination */
    if (elev->current_floor == elev->target_floor) {
        return TRUE;
    }
    /* 2. Intermediate stop: same-direction hallway/cabin request */
    if (Queue_Check_Stop(&elev->queue, elev->current_floor, elev->direction)) {
        /* Remove the intermediate floor and re-enqueue the final target
         * so the elevator continues after the doors cycle. */
        Queue_Remove_Floor(&elev->queue, elev->current_floor);
        Queue_Enqueue(&elev->queue, elev->target_floor, DIR_NONE);
        return TRUE;
    }
    return FALSE;
}

void Elevator_FSM_Update(elevator_t *elev) {
    /* Safety: If emergency is active, force motor STOP */
    if (GPIO_Read_Pin(EMERGENCY_PORT_IDX, EMERGENCY_PIN) == FALSE) {
        elev->state = STATE_EMERGENCY_STOP;
        elev->direction = DIR_NONE;
        Update_Motor_Speed(elev);
        return;
    } else if (elev->state == STATE_EMERGENCY_STOP) {
        elev->state = STATE_IDLE;
        Emergency_Reset();
    }

    /* Wait for timers (door operation, travel delay) */
    if (elev->state_timer > 0) {
        elev->state_timer--;
        return;
    }

    switch (elev->state) {
        case STATE_IDLE: {
            elevator_request_t req;
            if (Queue_Dequeue(&elev->queue, &req) == OK) {
                elev->target_floor = req.floor;
                if (elev->target_floor > elev->current_floor) {
                    elev->state = STATE_MOVING_UP;
                    elev->direction = DIR_UP;
                } else if (elev->target_floor < elev->current_floor) {
                    elev->state = STATE_MOVING_DOWN;
                    elev->direction = DIR_DOWN;
                } else {
                    /* Already here: just open doors */
                    elev->state = STATE_DOORS_OPENING;
                    elev->state_timer = DOOR_OPERATION_TIME;
                }
                Update_Motor_Speed(elev);
            }
            break;
        }

        case STATE_MOVING_UP:
            /* Advance one floor per timer tick */
            elev->current_floor++;
            if (Should_Stop_At(elev)) {
                elev->state = STATE_DOORS_OPENING;
                elev->state_timer = DOOR_OPERATION_TIME;
                Update_Motor_Speed(elev);
            } else {
                elev->state_timer = TRAVEL_TIME_PER_FLOOR;
            }
            break;

        case STATE_MOVING_DOWN:
            /* Descend one floor per timer tick */
            elev->current_floor--;
            if (Should_Stop_At(elev)) {
                elev->state = STATE_DOORS_OPENING;
                elev->state_timer = DOOR_OPERATION_TIME;
                Update_Motor_Speed(elev);
            } else {
                elev->state_timer = TRAVEL_TIME_PER_FLOOR;
            }
            break;

        case STATE_DOORS_OPENING:
            elev->state = STATE_DOORS_OPEN;
            elev->state_timer = DOOR_OPEN_TIME;
            break;

        case STATE_DOORS_OPEN:
            elev->state = STATE_DOORS_CLOSING;
            elev->state_timer = DOOR_OPERATION_TIME;
            break;

        case STATE_DOORS_CLOSING:
            elev->state = STATE_IDLE;
            elev->direction = DIR_NONE;
            Update_Motor_Speed(elev);
            break;

        default:
            break;
    }
}

void Elevator_Handle_Request(elevator_t *elev, uint8_t floor, direction_t dir) {
    /* SCAN Algorithm: Insert request in travel-direction order so the queue
     * always reflects logical service order, not insertion order.
     *   Going DOWN → sort descending (highest floor served first)
     *   Going UP   → sort ascending  (lowest floor served first)
     *   IDLE       → plain FIFO append */

    request_queue_t *q = &elev->queue;

    if (q->count >= QUEUE_SIZE) return;

    /* Duplicate check */
    for (uint8_t i = 0; i < q->count; i++) {
        uint8_t idx = (q->tail + i) % QUEUE_SIZE;
        if (q->requests[idx].floor == floor &&
            q->requests[idx].direction == dir) {
            return; /* already queued */
        }
    }

    /* Find sorted insertion position */
    uint8_t insert_pos = q->count; /* default: append */
    for (uint8_t i = 0; i < q->count; i++) {
        uint8_t idx = (q->tail + i) % QUEUE_SIZE;
        uint8_t ef  = q->requests[idx].floor;

        if (elev->direction == DIR_DOWN && floor > ef) {
            insert_pos = i; break; /* higher floor goes first when going DOWN */
        }
        if (elev->direction == DIR_UP && floor < ef) {
            insert_pos = i; break; /* lower floor goes first when going UP */
        }
    }

    /* Shift elements from insert_pos onward to make a gap */
    for (uint8_t i = q->count; i > insert_pos; i--) {
        uint8_t dst = (q->tail + i)     % QUEUE_SIZE;
        uint8_t src = (q->tail + i - 1) % QUEUE_SIZE;
        q->requests[dst] = q->requests[src];
    }

    /* Write new request at the insertion position */
    uint8_t pos = (q->tail + insert_pos) % QUEUE_SIZE;
    q->requests[pos].floor     = floor;
    q->requests[pos].direction = dir;
    q->head = (q->tail + q->count + 1) % QUEUE_SIZE;
    q->count++;
}
