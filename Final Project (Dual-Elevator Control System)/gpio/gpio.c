/**
 * @file gpio.c
 * @brief GPIO module implementation using port indices.
 */

#include "gpio.h"
#include "gpio_private.h"

static GPIO_Regs_t* const gpio_ports[] = { GPIOA, GPIOB, GPIOC, GPIOD, GPIOE };

void GPIO_Init_Pin(uint8_t port_idx, uint8_t pin, gpio_mode_t mode, gpio_pupd_t pupd) {
    if (port_idx >= 5) return;
    GPIO_Regs_t *port = gpio_ports[port_idx];
    
    /* Set Mode */
    port->MODER &= ~(3U << GPIO_MODER_Pos(pin));
    port->MODER |= ((uint32_t)mode << GPIO_MODER_Pos(pin));
    
    /* Set Pull-up/down */
    port->PUPDR &= ~(3U << GPIO_PUPDR_Pos(pin));
    port->PUPDR |= ((uint32_t)pupd << GPIO_PUPDR_Pos(pin));
}

void GPIO_Set_AF(uint8_t port_idx, uint8_t pin, uint8_t af) {
    if (port_idx >= 5) return;
    GPIO_Regs_t *port = gpio_ports[port_idx];
    
    uint8_t idx = pin / 8;
    port->AFR[idx] &= ~(0xFU << GPIO_AFR_Pos(pin));
    port->AFR[idx] |= ((uint32_t)af << GPIO_AFR_Pos(pin));
}

void GPIO_Write_Pin(uint8_t port_idx, uint8_t pin, bool_t state) {
    if (port_idx >= 5) return;
    if (state) {
        gpio_ports[port_idx]->BSRR = (1U << pin);
    } else {
        gpio_ports[port_idx]->BSRR = (1U << (pin + 16));
    }
}

bool_t GPIO_Read_Pin(uint8_t port_idx, uint8_t pin) {
    if (port_idx >= 5) return (bool_t)0;
    return (gpio_ports[port_idx]->IDR & (1U << pin)) ? TRUE : FALSE;
}
