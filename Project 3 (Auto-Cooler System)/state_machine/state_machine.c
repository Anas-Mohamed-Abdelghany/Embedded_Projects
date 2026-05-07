/*
 * state_machine.c
 * Mealy State Machine — Auto-Cooler temperature control logic.
 *
 * Structure: switch(current_state) with guard conditions on temperature.
 * Each case computes outputs AND the next state — true Mealy model.
 *
 * All hardware outputs go through driver APIs:
 *   Pwm_SetDuty()    — fan speed via TIM3
 *   Gpio_WritePin()  — alarm LED on PC0
 *   LCD_SetCursor()  — flicker-free display update (reposition, not clear)
 *   LCD_WriteString()
 *
 * LCD character 0xDF = degree symbol (°) in HD44780 CGROM A00 table.
 */

#include "state_machine.h"
#include "state_machine_private.h" // Internal definitions
#include "pwm.h"
#include "gpio.h"
#include "lcd.h"
#include <stdio.h>

/* Module-level state variable */
static SystemState current_state = STATE_IDLE;

/* ================================================================== */
/* Private helpers                                                      */
/* ================================================================== */

/*
 * get_duty — maps temperature to PWM duty cycle (normal operation only).
 * Does NOT handle the overheat override — that is done in FSM_Update().
 */
static uint8_t get_duty(float temp)
{
    if      (temp < T_COOL_ENTRY)  return DUTY_OFF;
    else if (temp < T_SPEED_MED)   return DUTY_LOW;
    else if (temp < T_SPEED_FULL)  return DUTY_MED;
    else                           return DUTY_FULL;
}

/*
 * update_display — refreshes both LCD lines without clearing the screen.
 *
 * Uses LCD_SetCursor() + fixed-width strings (16 chars + null) so old
 * characters are overwritten directly — no blank flash (Tutorial 5 §6).
 *
 * Line 1: "Temp: XX.X°C    "  (°C = 0xDF + 'C')
 * Line 2: "Fan:   XXX%     "  or  "!! OVERHEAT !!  "
 */
static void update_display(float temp, uint8_t duty, uint8_t overheat)
{
    char buf[17];
    int t_int = (int)temp; // Convert to whole number (e.g., 24)

    // Line 1: Use %d (Integer) instead of %f
    snprintf(buf, 17, "Temp: %d C      ", t_int);
    LCD_SetCursor(0, 0);
    LCD_WriteString(buf);

    // Line 2
    LCD_SetCursor(1, 0);
    if (overheat) {
        LCD_WriteString("!! OVERHEAT !!  ");
    } else {
        // Use %d for duty
        snprintf(buf, 17, "Fan:   %d %%     ", (int)duty);
        LCD_WriteString(buf);
    }
}

/* ================================================================== */
/* FSM_Init                                                             */
/* Sets initial state, turns fan OFF, clears alarm LED.               */
/* Must be called after GPIO, PWM, and LCD drivers are initialised.   */
/* ================================================================== */
void FSM_Init(void)
{
    /* Alarm LED: PC0 output push-pull, start LOW */
    Gpio_Init(ALARM_PORT, ALARM_PIN, GPIO_OUTPUT, GPIO_PUSH_PULL);
    Gpio_WritePin(ALARM_PORT, ALARM_PIN, LOW);

    current_state = STATE_IDLE;
    Pwm_SetDuty(DUTY_OFF);
}

/* ================================================================== */
/* FSM_Update                                                           */
/* Called from the main loop on every new ADC temperature reading.    */
/* Computes outputs and next state — classic Mealy machine structure.  */
/* ================================================================== */
void FSM_Update(float temperature)
{
    uint8_t duty;

    switch (current_state)
    {
        /* ========================================================== */
        case STATE_IDLE:
        /* ========================================================== */

            if (temperature < T_COOL_ENTRY)
            {
                Pwm_SetDuty(DUTY_OFF);
                update_display(temperature, DUTY_OFF, 0);
            }
            else
            {
                duty = get_duty(temperature);
                Pwm_SetDuty(duty);
                update_display(temperature, duty, 0);
                current_state = STATE_COOLING;
            }
            break;

        /* ========================================================== */
        case STATE_COOLING:
        /* ========================================================== */

            if (temperature >= T_OVERHEAT)
            {
                Pwm_SetDuty(DUTY_FULL);
                Gpio_WritePin(ALARM_PORT, ALARM_PIN, HIGH);
                update_display(temperature, DUTY_FULL, 1);
                current_state = STATE_OVERHEAT;
            }
            else if (temperature >= T_COOL_ENTRY)
            {
                duty = get_duty(temperature);
                Pwm_SetDuty(duty);
                update_display(temperature, duty, 0);
            }
            else
            {
                Pwm_SetDuty(DUTY_OFF);
                update_display(temperature, DUTY_OFF, 0);
                current_state = STATE_IDLE;
            }
            break;

        /* ========================================================== */
        case STATE_OVERHEAT:
        /* ========================================================== */

            if (temperature >= T_OVERHEAT)
            {
                Pwm_SetDuty(DUTY_FULL);
                Gpio_WritePin(ALARM_PORT, ALARM_PIN, HIGH);
                update_display(temperature, DUTY_FULL, 1);
            }
            else
            {
                Gpio_WritePin(ALARM_PORT, ALARM_PIN, LOW);
                duty = get_duty(temperature);
                Pwm_SetDuty(duty);
                update_display(temperature, duty, 0);
                current_state = STATE_COOLING;
            }
            break;

        default:
            /* Defensive: should never reach here */
            current_state = STATE_IDLE;
            Pwm_SetDuty(DUTY_OFF);
            Gpio_WritePin(ALARM_PORT, ALARM_PIN, LOW);
            break;
    }
}

/* ================================================================== */
/* FSM_GetState                                                         */
/* ================================================================== */
SystemState FSM_GetState(void)
{
    return current_state;
}