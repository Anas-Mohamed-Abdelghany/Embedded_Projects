/**
 * @file queue.c
 * @brief Queue module implementation.
 */

#include "queue.h"
#include "queue_private.h"

void Queue_Init(request_queue_t *q) {
    q->head = 0;
    q->tail = 0;
    q->count = 0;
}

status_t Queue_Enqueue(request_queue_t *q, uint8_t floor, direction_t dir) {
    if (q->count >= QUEUE_SIZE) return ERROR;
    
    /* Avoid exact duplicates (same floor AND same direction) */
    for (uint8_t i = 0; i < q->count; i++) {
        uint8_t idx = (q->tail + i) % QUEUE_SIZE;
        if (q->requests[idx].floor == floor && q->requests[idx].direction == dir) {
            return OK;
        }
    }

    q->requests[q->head].floor = floor;
    q->requests[q->head].direction = dir;
    q->head = (q->head + 1) % QUEUE_SIZE;
    q->count++;
    return OK;
}

status_t Queue_Dequeue(request_queue_t *q, elevator_request_t *req) {
    if (q->count == 0) return ERROR;
    
    *req = q->requests[q->tail];
    q->tail = (q->tail + 1) % QUEUE_SIZE;
    q->count--;
    return OK;
}

bool_t Queue_Check_Stop(request_queue_t *q, uint8_t floor, direction_t current_dir) {
    for (uint8_t i = 0; i < q->count; i++) {
        uint8_t idx = (q->tail + i) % QUEUE_SIZE;
        /* Stop if it's a cabin call (NONE) or matches current direction */
        if (q->requests[idx].floor == floor) {
            if (q->requests[idx].direction == DIR_NONE || 
                q->requests[idx].direction == current_dir) {
                return TRUE;
            }
        }
    }
    return FALSE;
}

bool_t Queue_Contains(request_queue_t *q, uint8_t floor) {
    for (uint8_t i = 0; i < q->count; i++) {
        uint8_t idx = (q->tail + i) % QUEUE_SIZE;
        if (q->requests[idx].floor == floor) return TRUE;
    }
    return FALSE;
}

void Queue_Remove_Floor(request_queue_t *q, uint8_t floor) {
    uint8_t new_count = 0;
    elevator_request_t temp_reqs[QUEUE_SIZE];
    
    for (uint8_t i = 0; i < q->count; i++) {
        uint8_t idx = (q->tail + i) % QUEUE_SIZE;
        if (q->requests[idx].floor != floor) {
            temp_reqs[new_count++] = q->requests[idx];
        }
    }
    
    for (uint8_t i = 0; i < new_count; i++) {
        q->requests[i] = temp_reqs[i];
    }
    q->tail = 0;
    q->head = new_count % QUEUE_SIZE;
    q->count = new_count;
}
