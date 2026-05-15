/**
 * @file queue.h
 * @brief Circular buffer queue for elevator requests.
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include "main/main.h"

/* Forward declare direction_t to avoid circular dependency */
typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN
} direction_t;

#define QUEUE_SIZE    8

typedef struct {
    uint8_t floor;
    direction_t direction;
} elevator_request_t;

typedef struct {
    elevator_request_t requests[QUEUE_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} request_queue_t;

void Queue_Init(request_queue_t *q);
status_t Queue_Enqueue(request_queue_t *q, uint8_t floor, direction_t dir);
status_t Queue_Dequeue(request_queue_t *q, elevator_request_t *req);
bool_t Queue_Check_Stop(request_queue_t *q, uint8_t floor, direction_t current_dir);
void Queue_Remove_Floor(request_queue_t *q, uint8_t floor);
bool_t Queue_Contains(request_queue_t *q, uint8_t floor);

#endif /* QUEUE_H */
