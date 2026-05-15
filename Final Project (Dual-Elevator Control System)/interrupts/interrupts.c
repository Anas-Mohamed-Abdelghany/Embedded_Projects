/**
 * @file interrupts.c
 * @brief Interrupt Service Routine implementations.
 */

#include "interrupts.h"
#include "interrupts_private.h"
#include "timer/timer_private.h"
#include "exti/exti_private.h"

void TIM2_IRQHandler(void) {
    if (TIM2->SR & TIM_SR_UIF) {
        TIM2->SR &= ~TIM_SR_UIF;
        Scheduler_Tick();
    }
}

void SPI1_IRQHandler(void) {
    SPI1_IRQHandler_Impl();
}

void USART1_IRQHandler(void) {
    USART1_IRQHandler_Impl();
}

void EXTI0_IRQHandler(void) { EXTI->PR = (1U << 0); EXTI_Handler_Impl(0); }
void EXTI1_IRQHandler(void) { EXTI->PR = (1U << 1); EXTI_Handler_Impl(1); }
void EXTI2_IRQHandler(void) { EXTI->PR = (1U << 2); EXTI_Handler_Impl(2); }
void EXTI3_IRQHandler(void) { EXTI->PR = (1U << 3); EXTI_Handler_Impl(3); }
void EXTI4_IRQHandler(void) { EXTI->PR = (1U << 4); EXTI_Handler_Impl(4); }
void EXTI9_5_IRQHandler(void) {
    uint32_t pending = EXTI->PR & 0x03E0; /* Pins 5-9 */
    EXTI->PR = pending; /* Clear all pending in this group */
    for (int i = 5; i <= 9; i++) {
        if (pending & (1U << i)) {
            EXTI_Handler_Impl(i);
        }
    }
}

void EXTI15_10_IRQHandler(void) {
    uint32_t pending = EXTI->PR & 0xFC00; /* Pins 10-15 */
    EXTI->PR = pending; /* Clear all pending in this group */
    for (int i = 10; i <= 15; i++) {
        if (pending & (1U << i)) {
            EXTI_Handler_Impl(i);
        }
    }
}
