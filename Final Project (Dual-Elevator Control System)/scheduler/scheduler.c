/**
 * @file scheduler.c
 * @brief Scheduler module implementation.
 */

#include "scheduler.h"
#include "scheduler_private.h"

static volatile uint32_t system_tick = 0;
static task_t task_list[MAX_TASKS];
static uint8_t task_count = 0;

void Scheduler_Init(void) {
    system_tick = 0;
    task_count = 0;
}

void Scheduler_Add_Task(task_func_t func, uint32_t interval_ms) {
    if (task_count < MAX_TASKS) {
        task_list[task_count].func = func;
        task_list[task_count].interval_ms = interval_ms;
        task_list[task_count].last_run = 0;
        task_list[task_count].ready = FALSE;
        task_count++;
    }
}

void Scheduler_Tick(void) {
    system_tick++;
    for (uint8_t i = 0; i < task_count; i++) {
        if ((system_tick - task_list[i].last_run) >= task_list[i].interval_ms) {
            task_list[i].ready = TRUE;
        }
    }
}

void Scheduler_Run(void) {
    for (uint8_t i = 0; i < task_count; i++) {
        if (task_list[i].ready) {
            Enter_Critical();
            task_list[i].ready = FALSE;
            task_list[i].last_run = system_tick;
            Exit_Critical();
            
            task_list[i].func();
        }
    }
}
