/**
 * @file scheduler.h
 * @brief Cooperative scheduler for STM32 dual-elevator system.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "main/main.h"
#include "exti/exti.h"

typedef void (*task_func_t)(void);

typedef struct {
    task_func_t func;
    uint32_t interval_ms;
    uint32_t last_run;
    bool_t ready;
} task_t;

/**
 * @brief Initializes the scheduler and core tasks.
 */
void Scheduler_Init(void);

/**
 * @brief Adds a task to the scheduler list.
 * @param func Function to execute
 * @param interval_ms Execution interval in milliseconds
 */
void Scheduler_Add_Task(task_func_t func, uint32_t interval_ms);

/**
 * @brief Main scheduler loop (to be called in while(1)).
 */
void Scheduler_Run(void);

/**
 * @brief Increments internal tick (to be called in TIM2 ISR).
 */
void Scheduler_Tick(void);

#endif /* SCHEDULER_H */
