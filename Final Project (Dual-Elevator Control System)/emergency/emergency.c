/**
 * @file emergency.c
 * @brief Emergency module implementation.
 */

#include "emergency.h"
#include "emergency_private.h"
#include "pwm/pwm.h"
#include "gpio/gpio.h"
#include "main/pin_mapping.h"

static volatile bool_t emergency_active = FALSE;
static volatile emergency_type_t current_emergency = EMERGENCY_NONE;

void Emergency_Init(void) {
    emergency_active = FALSE;
    current_emergency = EMERGENCY_NONE;
}

void Emergency_Trigger(emergency_type_t type) {
    Enter_Critical();
    emergency_active = TRUE;
    current_emergency = type;
    Exit_Critical();
    
    /* Immediate Hardware Action: Stop Motor and Brake */
    PWM_Stop();
    GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN1_PIN, FALSE);
    GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN2_PIN, FALSE);
}

bool_t Emergency_Is_Active(void) {
    return (GPIO_Read_Pin(EMERGENCY_PORT_IDX, EMERGENCY_PIN) == FALSE);
}

void Emergency_Reset(void) {
    Enter_Critical();
    emergency_active = FALSE;
    current_emergency = EMERGENCY_NONE;
    Exit_Critical();
}
