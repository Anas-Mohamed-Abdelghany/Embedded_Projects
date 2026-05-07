#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void Timer_Init(void);
void delay_ms(uint32_t ms);
void delay_us(uint32_t us);

#endif /* TIMER_H */
