#include "gpio.h"
#include <stdint.h>

/* ─────────────── REGISTER STRUCT ─────────────── */
typedef struct {
    volatile uint32_t MODER;   /* 0x00 - Mode Register */
    volatile uint32_t OTYPER;  /* 0x04 - Output Type */
    volatile uint32_t OSPEEDR; /* 0x08 - Output Speed */
    volatile uint32_t PUPDR;   /* 0x0C - Pull-up/down */
    volatile uint32_t IDR;     /* 0x10 - Input Data */
    volatile uint32_t ODR;     /* 0x14 - Output Data */
    volatile uint32_t BSRR;    /* 0x18 - Bit Set/Reset */
} GpioType;

/* Port Addresses */
#define GPIOA ((GpioType *) 0x40020000UL)
#define GPIOB ((GpioType *) 0x40020400UL)
#define GPIOC ((GpioType *) 0x40020800UL)
#define GPIOD ((GpioType *) 0x40020C00UL)
#define GPIOE ((GpioType *) 0x40021000UL)

/* Helper to get port from character */
static GpioType* Get_Port(char PortName) {
    switch(PortName) {
        case 'A': return GPIOA;
        case 'B': return GPIOB;
        case 'C': return GPIOC;
        case 'D': return GPIOD;
        case 'E': return GPIOE;
        default: return (GpioType*)0;
    }
}

/* ─────────────── DRIVER FUNCTIONS ─────────────── */
void Gpio_WritePin(char PortName, uint8_t PinNumber, uint8_t Data) {
    GpioType* port = Get_Port(PortName);
    if (Data) port->ODR |= (1UL << PinNumber);
    else      port->ODR &= ~(1UL << PinNumber);
}

uint8_t Gpio_ReadPin(char PortName, uint8_t PinNumber) {
    GpioType* port = Get_Port(PortName);
    return (port->IDR & (1UL << PinNumber)) ? 1 : 0;
}

void Gpio_Init(char PortName, uint8_t PinNumber, uint8_t PinMode, uint8_t DefaultState) {
    GpioType* port = Get_Port(PortName);
    if (!port) return;

    /* 1. Set Mode (00=Input, 01=Output) */
    port->MODER &= ~(3UL << (PinNumber * 2)); // Clear bits
    port->MODER |= ((uint32_t)PinMode << (PinNumber * 2));

    /* 2. Set Pull-up for Inputs or Initial State for Outputs */
    if (PinMode == GPIO_INPUT) {
        port->PUPDR &= ~(3UL << (PinNumber * 2));
        port->PUPDR |= (1UL << (PinNumber * 2)); // Enable Pull-up
    } else {
        Gpio_WritePin(PortName, PinNumber, DefaultState);
    }
}
