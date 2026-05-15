/**
 * @file rcc.c
 * @brief Implementation of RCC module.
 */

#include "rcc.h"
#include "rcc_private.h"

void RCC_Init(void) {
    /* Enable HSI (High Speed Internal) clock */
    RCC->CR |= RCC_CR_HSION;
    
    /* Wait until HSI is ready */
    while(!(RCC->CR & RCC_CR_HSIRDY));
    
    /* Configure AHB/APB Prescalers */
    /* HCLK = SYSCLK / 1 = 16MHz */
    RCC->CFGR &= ~RCC_CFGR_HPRE;
    /* PCLK1 (APB1) = HCLK / 1 = 16MHz */
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    /* PCLK2 (APB2) = HCLK / 1 = 16MHz */
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    
    /* Select HSI as system clock source */
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_HSI;
    
    /* Wait until HSI is used as system clock source */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);
    
    /* Enable SYSCFG clock for EXTI */
    RCC->APB2ENR |= (1 << RCC_APB2ENR_SYSCFGEN_Pos);
}

void RCC_GPIO_Enable(char port) {
    switch(port) {
        case 'A': RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos); break;
        case 'B': RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOBEN_Pos); break;
        case 'C': RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOCEN_Pos); break;
        case 'D': RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIODEN_Pos); break;
        case 'E': RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOEEN_Pos); break;
        default: break;
    }
}

void RCC_SPI1_Enable(void) {
    RCC->APB2ENR |= (1 << RCC_APB2ENR_SPI1EN_Pos);
}

void RCC_USART1_Enable(void) {
    RCC->APB2ENR |= (1 << RCC_APB2ENR_USART1EN_Pos);
}

void RCC_Timers_Enable(void) {
    RCC->APB1ENR |= (1 << RCC_APB1ENR_TIM2EN_Pos);
    RCC->APB1ENR |= (1 << RCC_APB1ENR_TIM3EN_Pos);
}
