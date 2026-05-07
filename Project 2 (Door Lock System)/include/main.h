#ifndef MAIN_H
#define MAIN_H

#include <stdint.h>
#include <string.h>
// Note: No HAL include here!

/* ─────────────── PIN DEFINITIONS ─────────────── */

/* 4x3 Keypad – Rows (OUTPUT) */
#define KP_ROW0_PIN   0      
#define KP_ROW1_PIN   1
#define KP_ROW2_PIN   2
#define KP_ROW3_PIN   3
#define KP_ROW_PORT   'A'   

/* 4x3 Keypad – Columns (INPUT) */
#define KP_COL0_PIN   4
#define KP_COL1_PIN   5
#define KP_COL2_PIN   6
#define KP_COL_PORT   'A'

/* 7-Segment display – segments a-g */
#define SEG_A_PIN     0
#define SEG_B_PIN     1
#define SEG_C_PIN     2
#define SEG_D_PIN     3
#define SEG_E_PIN     4
#define SEG_F_PIN     5
#define SEG_G_PIN     6
#define SEG_PORT      'B' 

/* Status LEDs – PD0, PD1, PD2 */
#define LED_SUCCESS_PIN  0
#define LED_ALARM_PIN    1
#define LED_BELL_PIN     2
#define LED_STATUS_PORT  'D' 

/* External buttons – PE0, PE1, PE2 */
#define BTN_RESET_PIN    0
#define BTN_BELL_PIN     1
#define BTN_LOCK_PIN     2
#define BTN_PORT         'E' 

/* ─────────────── SYSTEM CONSTANTS ─────────────── */
#define PASSWORD_LENGTH       4
#define LOCKOUT_THRESHOLD     3
#define BELL_PULSE_MS         300

/* Values for Logic levels */
#define HIGH    0x01U
#define LOW     0x00U

/* Secret passcode */
static const char SECRET_CODE[PASSWORD_LENGTH] = {'1', '2', '3', '4'};

#endif