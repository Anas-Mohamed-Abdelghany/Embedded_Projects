/**
 * @file exti.c
 * @brief EXTI module implementation.
 */

#include "exti.h"
#include "exti_private.h"
#include "interrupts/interrupts_private.h"

void EXTI_Init_Pin(uint8_t port_idx, uint8_t pin, exti_trigger_t trigger) {
    /* 1. Map GPIO to EXTI line in SYSCFG */
    SYSCFG->EXTICR[SYSCFG_EXTICR_Index(pin)] &= ~(0xFU << SYSCFG_EXTICR_Pos(pin));
    SYSCFG->EXTICR[SYSCFG_EXTICR_Index(pin)] |= ((uint32_t)port_idx << SYSCFG_EXTICR_Pos(pin));
    
    /* 2. Configure Trigger */
    EXTI->RTSR &= ~(1U << pin);
    EXTI->FTSR &= ~(1U << pin);
    
    if (trigger == EXTI_TRIGGER_RISING || trigger == EXTI_TRIGGER_BOTH) {
        EXTI->RTSR |= (1U << pin);
    }
    if (trigger == EXTI_TRIGGER_FALLING || trigger == EXTI_TRIGGER_BOTH) {
        EXTI->FTSR |= (1U << pin);
    }
    
    /* 3. Unmask EXTI line */
    EXTI->IMR |= (1U << pin);
}

void EXTI_Enable_IRQ(uint8_t pin, uint8_t priority) {
    IRQn_Type irqn;
    
    if (pin == 0) irqn = IRQ_EXTI0;
    else if (pin == 1) irqn = IRQ_EXTI1;
    else if (pin == 2) irqn = IRQ_EXTI2;
    else if (pin == 3) irqn = IRQ_EXTI3;
    else if (pin == 4) irqn = IRQ_EXTI4;
    else if (pin <= 9) irqn = IRQ_EXTI9_5;
    else irqn = IRQ_EXTI15_10;
    
    NVIC_Set_Priority(irqn, priority);
    NVIC_Enable_IRQ(irqn);
}

void EXTI_Clear_Pending(uint8_t pin) {
    EXTI->PR = (1U << pin);
}
