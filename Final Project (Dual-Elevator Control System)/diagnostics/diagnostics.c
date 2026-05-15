/**
 * @file diagnostics.c
 * @brief Diagnostics module implementation.
 */

#include "diagnostics.h"
#include "diagnostics_private.h"
#include "queue/queue.h"
#include "main/main.h"
#include "uart/uart.h"
#include "emergency/emergency.h"
#include <stdarg.h>
#include "gpio/gpio.h"
#include "main/pin_mapping.h"

/* Private lightweight snprintf implementation (No standard headers/syscalls needed) */
static int mini_snprintf(char* buffer, uint32_t size, const char* format, ...) {
    va_list args;
    va_start(args, format);
    char *p = buffer;
    char *end = buffer + size - 1;
    
    while (*format && p < end) {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                int32_t val = va_arg(args, int32_t);
                if (val < 0) { if (p < end) *p++ = '-'; val = -val; }
                char temp[11]; uint8_t i = 0;
                if (val == 0) { temp[i++] = '0'; }
                while (val > 0) { temp[i++] = (val % 10) + '0'; val /= 10; }
                while (i > 0 && p < end) { *p++ = temp[--i]; }
            } else if (*format == 's') {
                char *s = va_arg(args, char*);
                while (*s && p < end) { *p++ = *s++; }
            }
        } else {
            *p++ = *format;
        }
        format++;
    }
    *p = '\0';
    va_end(args);
    return (int32_t)(p - buffer);
}

void Diagnostics_Init(void) {
    /* Initialize diagnostics state */
}

void Diagnostics_Send_Telemetry(elevator_t *elev) {
    /* Only print when something actually changes */
    static uint8_t last_fl    = 0xFF;
    static uint8_t last_tgt   = 0xFF;
    static uint8_t last_st    = 0xFF;
    static uint8_t last_dir   = 0xFF;
    static uint8_t last_q     = 0xFF;
    static uint8_t last_em    = 0xFF;
    static int8_t  last_sens  = -99;

    /* Read sensors with double-sample validation */
    int8_t active_sensor = -1;
    for (int i = 0; i < 4; i++) {
        uint8_t pin_idx = SENSOR_0_PIN + i;
        if (GPIO_Read_Pin(SENSOR_PORT_IDX, pin_idx) == FALSE &&
            GPIO_Read_Pin(SENSOR_PORT_IDX, pin_idx) == FALSE) {
            active_sensor = i + 1;
            break;
        }
    }

    uint8_t cur_em = Emergency_Is_Active();

    /* Skip if nothing changed */
    if (elev->current_floor == last_fl  &&
        elev->target_floor  == last_tgt &&
        (uint8_t)elev->state == last_st &&
        (uint8_t)elev->direction == last_dir &&
        elev->queue.count   == last_q   &&
        cur_em              == last_em  &&
        active_sensor       == last_sens) {
        return;
    }

    /* Update last-seen snapshot */
    last_fl   = elev->current_floor;
    last_tgt  = elev->target_floor;
    last_st   = (uint8_t)elev->state;
    last_dir  = (uint8_t)elev->direction;
    last_q    = elev->queue.count;
    last_em   = cur_em;
    last_sens = active_sensor;

    char msg[128];

    /* Build queue string */
    char q_str[32];
    char *p = q_str;
    *p++ = '[';
    for (uint8_t i = 0; i < elev->queue.count; i++) {
        uint8_t idx = (elev->queue.tail + i) % QUEUE_SIZE;
        uint8_t fl  = elev->queue.requests[idx].floor;
        direction_t dr = elev->queue.requests[idx].direction;
        if (fl >= 10) { *p++ = '0' + (fl / 10); }
        *p++ = '0' + (fl % 10);
        if      (dr == DIR_UP)   *p++ = 'U';
        else if (dr == DIR_DOWN) *p++ = 'D';
        else                     *p++ = 'C';
        if (i < elev->queue.count - 1) *p++ = ',';
    }
    *p++ = ']'; *p = '\0';

    /* Context-aware sensor validation:
     * Cross-check the raw sensor reading against elevator state/direction.
     * A sensor that doesn't match the current context is shown as "--". */
    char sens_str[4];
    bool_t sens_valid = FALSE;

    if (active_sensor > 0) {
        switch (elev->state) {
            case STATE_MOVING_UP:
                /* Sensor must be ahead of or at current position */
                sens_valid = (active_sensor >= elev->current_floor);
                break;
            case STATE_MOVING_DOWN:
                /* Sensor must be behind or at current position */
                sens_valid = (active_sensor <= elev->current_floor);
                break;
            case STATE_IDLE:
            case STATE_DOORS_OPENING:
            case STATE_DOORS_OPEN:
            case STATE_DOORS_CLOSING:
                /* Must exactly match current floor */
                sens_valid = (active_sensor == elev->current_floor);
                break;
            default:
                sens_valid = FALSE;
                break;
        }
    }

    if (sens_valid) {
        sens_str[0] = 'F';
        sens_str[1] = '0' + active_sensor;
        sens_str[2] = '\0';
    } else {
        sens_str[0] = '-'; sens_str[1] = '-'; sens_str[2] = '\0';
    }

    mini_snprintf(msg, sizeof(msg),
                  "ID:%d FL:%d TGT:%d ST:%d DIR:%d Q:%d%s EM:%d SENS:%s\r\n",
                  elev->id, elev->current_floor, elev->target_floor,
                  (int)elev->state, (int)elev->direction,
                  elev->queue.count, q_str, cur_em, sens_str);

    UART_Send_String(msg);
}

