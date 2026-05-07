#include "rcc.h"
#include <stdint.h>

#define RCC_AHB1ENR (*((volatile uint32_t *)(0x40023800 + 0x30)))

void Rcc_Init(void) {
    /* Basic init if needed, usually empty for internal 16MHz clock */
}

void Rcc_Enable(char portName) {
    switch(portName) {
        case 'A': RCC_AHB1ENR |= (1 << 0); break;
        case 'B': RCC_AHB1ENR |= (1 << 1); break;
        case 'C': RCC_AHB1ENR |= (1 << 2); break;
        case 'D': RCC_AHB1ENR |= (1 << 3); break;
        case 'E': RCC_AHB1ENR |= (1 << 4); break;
    }
}
