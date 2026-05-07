#include "keypad.h"
#include "main.h"
#include "gpio.h"
#include "rcc.h"
#include "state_machine.h"
#include <string.h>

/* ─────────────── KEYMAP (4 rows × 3 cols) ─────────────── */
static const char KEYMAP[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

/* 2D Array {Port, PinNumber} */
static const char rowPins[4][2] = {
    {KP_ROW_PORT, KP_ROW0_PIN},
    {KP_ROW_PORT, KP_ROW1_PIN},
    {KP_ROW_PORT, KP_ROW2_PIN},
    {KP_ROW_PORT, KP_ROW3_PIN}
};

static const char colPins[3][2] = {
    {KP_COL_PORT, KP_COL0_PIN},
    {KP_COL_PORT, KP_COL1_PIN},
    {KP_COL_PORT, KP_COL2_PIN}
};

/* Private state */
static char    last_key  = 0;

/* External delay function */
extern void delay_ms(uint32_t ms);

/* ─────────────── INIT ─────────────── */
void Keypad_Init(void)
{
    Rcc_Enable('A'); // Keypad is on Port A

    /* Init Rows as Output, Push-Pull, starting HIGH */
    for (uint8_t r = 0; r < 4; r++) {
        Gpio_Init(rowPins[r][0], rowPins[r][1], GPIO_OUTPUT, HIGH);
    }

    /* Init Columns as Input with Pull-Up */
    for (uint8_t c = 0; c < 3; c++) {
        Gpio_Init(colPins[c][0], colPins[c][1], GPIO_INPUT, HIGH); 
    }
}

/* ─────────────── SCAN ─────────────── */
char Keypad_Scan(void)
{
    for (uint8_t r = 0; r < 4; r++)
    {
        /* Activate row: Drive LOW */
        Gpio_WritePin(rowPins[r][0], rowPins[r][1], LOW);

        for (uint8_t c = 0; c < 3; c++)
        {
            /* Check if column is LOW */
            if (Gpio_ReadPin(colPins[c][0], colPins[c][1]) == LOW)
            {
                /* Wait 30ms and check again */
                delay_ms(30); 
                
                if (Gpio_ReadPin(colPins[c][0], colPins[c][1]) == LOW)
                {
                    /* Key confirmed. Deactivate row and return key */
                    Gpio_WritePin(rowPins[r][0], rowPins[r][1], HIGH);
                    return KEYMAP[r][c];
                }
            }
        }

        /* Deactivate row: Back to HIGH */
        Gpio_WritePin(rowPins[r][0], rowPins[r][1], HIGH);
    }
    return 0;
}

/* ─────────────── UPDATE ─────────────── */
static uint8_t keys_pressed_in_attempt = 0; // Track total clicks
static uint8_t correct_keys_in_attempt = 0; // Track correct clicks
/* ─────────────── UPDATE in keypad.c ─────────────── */
void Keypad_Update(void)
{
    if (SM_GetState() != STATE_LOCKED) {
        keys_pressed_in_attempt = 0;
        correct_keys_in_attempt = 0;
        return;
    }

    char key = Keypad_Scan();
    if (key == 0 || key == last_key) {
        if (key == 0) last_key = 0;
        return;
    }
    last_key = key;

    // Only accept numbers 0-9
    if (key < '0' || key > '9') return;

    keys_pressed_in_attempt++;

    /* Send an event for EVERY key press to update the progress bar */
    SM_ProcessEvent(EVENT_KEY_PRESSED); 

    // Now just check if it was the correct digit internally
    if (key == SECRET_CODE[keys_pressed_in_attempt - 1]) {
        correct_keys_in_attempt++;
    }

    // ONLY check for success or failure once the FULL length is reached
    if (keys_pressed_in_attempt >= PASSWORD_LENGTH) {
        if (correct_keys_in_attempt == PASSWORD_LENGTH) {
            SM_ProcessEvent(EVENT_SEQUENCE_COMPLETE);
        } else {
            SM_ProcessEvent(EVENT_INVALID_KEY);
        }
        
        keys_pressed_in_attempt = 0;
        correct_keys_in_attempt = 0;
    }
}