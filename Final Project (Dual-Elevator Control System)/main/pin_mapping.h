/**
 * @file pin_mapping.h
 * @brief Hardware pin mapping for STM32 dual-elevator system.
 */

#ifndef PIN_MAPPING_H
#define PIN_MAPPING_H

/* --- Port Indices --- */
#define PORT_A              0
#define PORT_B              1
#define PORT_C              2
#define PORT_D              3
#define PORT_E              4

/* --- Pin Mapping --- */
/* Floor Sensors (EXTI0-EXTI3) */
#define SENSOR_PORT_IDX     PORT_A
#define SENSOR_0_PIN        0
#define SENSOR_1_PIN        1
#define SENSOR_2_PIN        2
#define SENSOR_3_PIN        3

/* Cabin Buttons (EXTI4-EXTI7) */
#define CABIN_PORT_IDX      PORT_B
#define CABIN_BTN_0_PIN     4
#define CABIN_BTN_1_PIN     5
#define CABIN_BTN_2_PIN     6
#define CABIN_BTN_3_PIN     7

/* Hallway Buttons (Master Only) (EXTI10-EXTI15) */
#define HALL_PORT_IDX       PORT_C
#define HALL_BTN_0_PIN      10
#define HALL_BTN_1_PIN      11
#define HALL_BTN_2_PIN      12
#define HALL_BTN_3_PIN      13
#define HALL_BTN_4_PIN      14
#define HALL_BTN_5_PIN      15

/* Status LED (PB1) */
#define LED_PORT_IDX        PORT_B
#define LED_PIN             1

/* Emergency Stop (EXTI8) */
#define EMERGENCY_PORT_IDX  PORT_B
#define EMERGENCY_PIN       8

/* PWM Motor EN Pin (TIM3 CH3 → PB0) */
#define MOTOR_PORT_IDX      PORT_B
#define MOTOR_PIN           0
#define MOTOR_AF            2

/* Motor Direction Pins (L298N IN1/IN2) */
#define MOTOR_DIR_PORT_IDX  PORT_C
#define MOTOR_IN1_PIN       0        /* PC0 → IN1 driver */
#define MOTOR_IN2_PIN       1        /* PC1 → IN2 driver */

/* SPI1 IPC */
#define SPI1_PORT_IDX       PORT_A
#define SPI1_SCK_PIN        5
#define SPI1_MISO_PIN       6
#define SPI1_MOSI_PIN       7
#define SPI1_NSS_PIN        4
#define SPI1_AF             5

/* USART1 Telemetry */
#define UART1_PORT_IDX      PORT_A
#define UART1_TX_PIN        9
#define UART1_RX_PIN        10
#define UART1_AF            7

#endif /* PIN_MAPPING_H */
