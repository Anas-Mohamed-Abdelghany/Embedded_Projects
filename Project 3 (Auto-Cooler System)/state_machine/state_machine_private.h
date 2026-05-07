#ifndef STATE_MACHINE_PRIVATE_H
#define STATE_MACHINE_PRIVATE_H

#include <stdint.h>

/* ================================================================== */
/*        Internal constants, thresholds, and pin definitions         */
/* ================================================================== */

/* ---- Alarm LED hardware ---- */
#define ALARM_PORT   GPIO_C
#define ALARM_PIN    0              /* PC0 — active HIGH */

/* ---- Temperature thresholds (°C) ---- */
#define T_COOL_ENTRY   25.0f       /* IDLE → COOLING boundary           */
#define T_SPEED_MED    30.0f       /* 33% → 66% boundary               */
#define T_SPEED_FULL   35.0f       /* 66% → 100% boundary              */
#define T_OVERHEAT     40.0f       /* COOLING → OVERHEAT boundary       */

/* ---- Duty cycle constants (%) ---- */
#define DUTY_OFF    0U
#define DUTY_LOW   33U
#define DUTY_MED   66U
#define DUTY_FULL 100U


/* ================================================================== */
/*                Private function prototypes                         */
/* ================================================================== */

static uint8_t get_duty(float temp);

static void update_display(float temp, uint8_t duty, uint8_t overheat);


#endif /* STATE_MACHINE_PRIVATE_H */