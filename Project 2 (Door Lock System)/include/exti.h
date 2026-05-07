#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

/* Define the callback type */
typedef void (*ExtiCallback_t)(void);

/* Define trigger edges */
typedef enum { RISING, FALLING, BOTH } ExtiEdge_t;

/* Public API */
void Interrupts_Init(void); 
void Exti_EnableInterrupt(uint8_t lineNumber, char portName, ExtiEdge_t edge, ExtiCallback_t callback);
void Exti_Disable(uint8_t lineNumber);
void Exti_Enable(uint8_t lineNumber);

#endif
