/**
 * @file main_private.h
 * @brief Private definitions and Pin Mapping for Main module.
 */

#ifndef MAIN_PRIVATE_H
#define MAIN_PRIVATE_H

#include "main.h"
#include "pin_mapping.h"

/* Local prototypes */
static void System_Init(void);
static void Elevator_FSM_Update_Wrapper(void);
static void Diagnostics_Task(void);

#ifdef MASTER_MCU
static void Master_Task(void);
#else
static void Slave_Task(void);
#endif

#endif /* MAIN_PRIVATE_H */
