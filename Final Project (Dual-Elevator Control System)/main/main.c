/**
 * @file main.c
 * @brief Main application logic for STM32 dual-elevator system.
 */

#include "main.h"
#include "main_private.h"
#include "rcc/rcc.h"
#include "gpio/gpio.h"
#include "exti/exti.h"
#include "timer/timer.h"
#include "pwm/pwm.h"
#include "spi/spi.h"
#include "uart/uart.h"
#include "scheduler/scheduler.h"
#include "state_machine/elevator_fsm.h"
#include "state_machine/dispatcher.h"
#include "emergency/emergency.h"
#include "communication/communication.h"
#include "diagnostics/diagnostics.h"
#include "queue/queue.h"

/* Global Elevator Objects */
static elevator_t my_elevator;
#ifdef MASTER_MCU
static elevator_t remote_elevator;
#endif

int main(void) {
    System_Init();
    
    /* Power-On Signal: Turn LED ON immediately to prove we are alive */
    GPIO_Write_Pin(LED_PORT_IDX, LED_PIN, TRUE);
    
    while (1) {
        Scheduler_Run();
    }
}

static void System_Init(void) {
    RCC_Init();
    RCC_GPIO_Enable('A');
    RCC_GPIO_Enable('B');
    RCC_GPIO_Enable('C');
    RCC_SPI1_Enable();
    RCC_USART1_Enable();
    RCC_Timers_Enable();
    
    /* 1. Initialize PWM Motor Pin (PB0 → EN) */
    GPIO_Init_Pin(MOTOR_PORT_IDX, MOTOR_PIN, GPIO_MODE_AF, GPIO_PUPD_NONE);
    GPIO_Set_AF(MOTOR_PORT_IDX, MOTOR_PIN, MOTOR_AF);

    /* 1b. Initialize Motor Direction Pins (PC0=IN1, PC1=IN2) - both LOW = brake */
    GPIO_Init_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN1_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
    GPIO_Init_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN2_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
    GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN1_PIN, FALSE);
    GPIO_Write_Pin(MOTOR_DIR_PORT_IDX, MOTOR_IN2_PIN, FALSE);
    
    /* 2. Initialize Status LED Pin (PB1) - starts OFF */
    GPIO_Init_Pin(LED_PORT_IDX, LED_PIN, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE);
    GPIO_Write_Pin(LED_PORT_IDX, LED_PIN, FALSE);

    /* 3. Initialize SPI Pins */
    GPIO_Init_Pin(SPI1_PORT_IDX, SPI1_SCK_PIN, GPIO_MODE_AF, GPIO_PUPD_NONE);
    GPIO_Init_Pin(SPI1_PORT_IDX, SPI1_MISO_PIN, GPIO_MODE_AF, GPIO_PUPD_NONE);
    GPIO_Init_Pin(SPI1_PORT_IDX, SPI1_MOSI_PIN, GPIO_MODE_AF, GPIO_PUPD_NONE);
    GPIO_Set_AF(SPI1_PORT_IDX, SPI1_SCK_PIN, SPI1_AF);
    GPIO_Set_AF(SPI1_PORT_IDX, SPI1_MISO_PIN, SPI1_AF);
    GPIO_Set_AF(SPI1_PORT_IDX, SPI1_MOSI_PIN, SPI1_AF);
    
    /* 4. Initialize UART Pins */
    GPIO_Init_Pin(UART1_PORT_IDX, UART1_TX_PIN, GPIO_MODE_AF, GPIO_PUPD_NONE);
    GPIO_Set_AF(UART1_PORT_IDX, UART1_TX_PIN, UART1_AF);

    /* 5. Initialize Sensor Pins (EXTI 0-3) - Added Pull-ups to prevent floating */
    for (uint8_t i = 0; i <= 3; i++) {
        GPIO_Init_Pin(SENSOR_PORT_IDX, i, GPIO_MODE_INPUT, GPIO_PUPD_UP);
    }
    EXTI_Init_Pin(SENSOR_PORT_IDX, SENSOR_0_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(SENSOR_PORT_IDX, SENSOR_1_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(SENSOR_PORT_IDX, SENSOR_2_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(SENSOR_PORT_IDX, SENSOR_3_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Enable_IRQ(SENSOR_0_PIN, 3);
    EXTI_Enable_IRQ(SENSOR_1_PIN, 3);
    EXTI_Enable_IRQ(SENSOR_2_PIN, 3);
    EXTI_Enable_IRQ(SENSOR_3_PIN, 3);

    /* 6. Initialize Cabin Button Pins (EXTI 4-7) - Added Pull-ups */
    for (uint8_t i = 4; i <= 7; i++) {
        GPIO_Init_Pin(CABIN_PORT_IDX, i, GPIO_MODE_INPUT, GPIO_PUPD_UP);
    }
    EXTI_Init_Pin(CABIN_PORT_IDX, CABIN_BTN_0_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(CABIN_PORT_IDX, CABIN_BTN_1_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(CABIN_PORT_IDX, CABIN_BTN_2_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(CABIN_PORT_IDX, CABIN_BTN_3_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Enable_IRQ(CABIN_BTN_0_PIN, 4);
    EXTI_Enable_IRQ(CABIN_BTN_1_PIN, 4);
    EXTI_Enable_IRQ(CABIN_BTN_2_PIN, 4);
    EXTI_Enable_IRQ(CABIN_BTN_3_PIN, 4);

    /* 7. Initialize Emergency Stop (EXTI 8) - Added Pull-up */
    GPIO_Init_Pin(EMERGENCY_PORT_IDX, EMERGENCY_PIN, GPIO_MODE_INPUT, GPIO_PUPD_UP);
    EXTI_Init_Pin(EMERGENCY_PORT_IDX, EMERGENCY_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Enable_IRQ(EMERGENCY_PIN, 0);

    #ifdef MASTER_MCU
    /* 8. Initialize Hallway Buttons (EXTI 10-15) - Added Pull-ups */
    for (uint8_t i = 10; i <= 15; i++) {
        GPIO_Init_Pin(HALL_PORT_IDX, i, GPIO_MODE_INPUT, GPIO_PUPD_UP);
    }
    EXTI_Init_Pin(HALL_PORT_IDX, HALL_BTN_0_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(HALL_PORT_IDX, HALL_BTN_1_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(HALL_PORT_IDX, HALL_BTN_2_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(HALL_PORT_IDX, HALL_BTN_3_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(HALL_PORT_IDX, HALL_BTN_4_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Init_Pin(HALL_PORT_IDX, HALL_BTN_5_PIN, EXTI_TRIGGER_FALLING);
    EXTI_Enable_IRQ(HALL_BTN_0_PIN, 5);
    #endif

    #ifdef MASTER_MCU
    SPI_Init_IPC(TRUE);
    Elevator_FSM_Init(&my_elevator, 1);
    Elevator_FSM_Init(&remote_elevator, 2);
    Dispatcher_Init();
    #else
    SPI_Init_IPC(FALSE);
    Elevator_FSM_Init(&my_elevator, 2);
    #endif
    
    PWM_Init_Motor();
    PWM_Start();
    UART_Init_Telemetry();
    
    #ifdef MASTER_MCU
    UART_Send_String_Blocking("\r\n>>> Elevator System: MASTER MCU STARTING <<<\r\n");
    #else
    UART_Send_String_Blocking("\r\n>>> Elevator System: SLAVE MCU STARTING <<<\r\n");
    #endif

    Timer_Init_Scheduler();
    Scheduler_Init();
    Emergency_Init();
    Communication_Init();
    
    #ifdef MASTER_MCU
    Scheduler_Add_Task(Master_Task, 100);
    #else
    Scheduler_Add_Task(Slave_Task, 50);
    #endif
    
    Scheduler_Add_Task((task_func_t)Elevator_FSM_Update_Wrapper, 1);
    Scheduler_Add_Task((task_func_t)Diagnostics_Task, 50);

    Timer_Start();
}

static void Elevator_FSM_Update_Wrapper(void) {
    Elevator_FSM_Update(&my_elevator);
    #ifdef MASTER_MCU
    Elevator_FSM_Update(&remote_elevator);
    #endif
}

static void Diagnostics_Task(void) {
    static bool_t led_state = FALSE;
    led_state = !led_state;
    GPIO_Write_Pin(LED_PORT_IDX, LED_PIN, led_state);
    
    Diagnostics_Send_Telemetry(&my_elevator);
}

#ifdef MASTER_MCU
static uint8_t comm_timeout_cnt = 0;
static void Master_Task(void) {
    static ipc_packet_t tx_packet, rx_packet;
    Communication_Prepare_Packet(&tx_packet);
    SPI_Transfer_Async((uint8_t *)&tx_packet, (uint8_t *)&rx_packet);
    
    if (Communication_Process_Packet(&rx_packet) == OK) {
        remote_elevator.state = rx_packet.state;
        remote_elevator.current_floor = rx_packet.current_floor;
        comm_timeout_cnt = 0;
    } else {
        comm_timeout_cnt++;
        if (comm_timeout_cnt > 10) { /* 500ms timeout */
            /* Comm Fault: Master takes all calls */
            // Handle fault state
        }
    }
}
#else
static void Slave_Task(void) {
    static ipc_packet_t tx_packet, rx_packet;
    tx_packet.state = my_elevator.state;
    tx_packet.current_floor = my_elevator.current_floor;
    Communication_Prepare_Packet(&tx_packet);
    SPI_Transfer_Async((uint8_t *)&tx_packet, (uint8_t *)&rx_packet);
    
    if (Communication_Process_Packet(&rx_packet) == OK) {
        if (rx_packet.target_floor != my_elevator.target_floor) {
            Elevator_Handle_Request(&my_elevator, rx_packet.target_floor, DIR_NONE);
        }
    } else {
        /* Comm Fault: Slave enters Independent Mode */
    }
}
#endif

void EXTI_Handler_Impl(uint8_t pin) {
    /* 1. Emergency Stop (Highest Priority) */
    if (pin == EMERGENCY_PIN) {
        Emergency_Trigger(EMERGENCY_STOP_PRESSED);
        return;
    }

    /* 2. Floor Sensors (A0-A3): TELEMETRY ONLY.
     * These are binary TRUE/FALSE hardware status signals.
     * FL is managed exclusively by the FSM timer.
     * No floor or state changes happen here. */
    if (pin >= SENSOR_0_PIN && pin <= SENSOR_3_PIN) {
        return;
    }

    /* 3. Cabin Buttons (Internal Requests) */
    if (pin >= CABIN_BTN_0_PIN && pin <= CABIN_BTN_3_PIN) {
        uint8_t req_floor = pin - CABIN_BTN_0_PIN + 1;
        /* Ignore if already at the requested floor */
        if (req_floor != my_elevator.current_floor) {
            Elevator_Handle_Request(&my_elevator, req_floor, DIR_NONE);
        }
        return;
    }

    /* 4. Hallway Buttons (Master Only Dispatching) */
    #ifdef MASTER_MCU
    if (pin >= HALL_BTN_0_PIN && pin <= HALL_BTN_5_PIN) {
        uint8_t floor = 1;
        direction_t dir = DIR_NONE;
        
        switch(pin) {
            case 10: floor = 1; dir = DIR_UP;   break; /* C10: F1 UP */
            case 11: floor = 2; dir = DIR_UP;   break; /* C11: F2 UP */
            case 12: floor = 2; dir = DIR_DOWN; break; /* C12: F2 DOWN */
            case 13: floor = 3; dir = DIR_DOWN; break; /* C13: F3 DOWN */
            case 14: floor = 3; dir = DIR_UP;   break; /* C14: F3 UP */
            case 15: floor = 4; dir = DIR_DOWN; break; /* C15: F4 DOWN */
            default: return;
        }
        /* Ignore hallway call if elevator is already idle at that floor */
        if (floor != my_elevator.current_floor || 
            my_elevator.state != STATE_IDLE) {
            Dispatcher_Allocate_Request(floor, dir, &my_elevator, &remote_elevator, FALSE);
        }
        return;
    }
    #endif
}
