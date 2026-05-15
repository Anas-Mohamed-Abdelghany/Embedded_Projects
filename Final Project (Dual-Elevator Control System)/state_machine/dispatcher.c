/**
 * @file dispatcher.c
 * @brief Dispatcher module implementation.
 */

#include "dispatcher.h"
#include "dispatcher_private.h"
#include <stdlib.h>

void Dispatcher_Init(void) {
    /* Initialization if needed */
}

static int Calculate_Score(uint8_t floor, direction_t call_dir, elevator_t *elev) {
    /* 1. Immediate Match */
    if (elev->current_floor == floor && elev->state == STATE_IDLE) {
        return SCORE_IMMEDIATE;
    }
    
    /* 2. Perfect Match: Same direction AND not passed yet */
    if (elev->direction == call_dir) {
        if (call_dir == DIR_UP && floor > elev->current_floor) return SCORE_PERFECT;
        if (call_dir == DIR_DOWN && floor < elev->current_floor) return SCORE_PERFECT;
        
        /* Same direction but passed */
        return SCORE_PASSED;
    }
    
    /* 3. Idle Selection */
    if (elev->state == STATE_IDLE) {
        int dist = abs((int)elev->current_floor - (int)floor);
        return SCORE_IDLE - dist;
    }
    
    return SCORE_OPPOSITE;
}

void Dispatcher_Allocate_Request(uint8_t floor, direction_t dir, elevator_t *elev_a, elevator_t *elev_b, bool_t comm_fault) {
    if (comm_fault) {
        Elevator_Handle_Request(elev_a, floor, dir);
        return;
    }

    int score_a = Calculate_Score(floor, dir, elev_a);
    int score_b = Calculate_Score(floor, dir, elev_b);
    
    if (score_a >= score_b) {
        Elevator_Handle_Request(elev_a, floor, dir);
    } else {
        Elevator_Handle_Request(elev_b, floor, dir);
    }
}
