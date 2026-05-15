/**
 * @file dispatcher_private.h
 * @brief Private definitions for Dispatcher module.
 */

#ifndef DISPATCHER_PRIVATE_H
#define DISPATCHER_PRIVATE_H

#include "dispatcher.h"

/* Scoring constants for priority matches */
#define SCORE_IMMEDIATE    100
#define SCORE_PERFECT      80
#define SCORE_IDLE         50
#define SCORE_PASSED       20
#define SCORE_OPPOSITE     0

#endif /* DISPATCHER_PRIVATE_H */
