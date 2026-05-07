/*
 * Peripheral IDs encode both the bus and the bit position:
 *   Bits [15:8] = bus index  (0x00=AHB1, 0x01=APB1, 0x02=APB2)
 *   Bits  [7:0] = bit number in the corresponding ENR register
 */

#ifndef RCC_H
#define RCC_H

#include <stdint.h>

/* ---------- AHB1 peripherals (RCC->AHB1ENR) ---------- */
#define RCC_GPIOA   0x0000U    /* AHB1, bit 0 */
#define RCC_GPIOB   0x0001U    /* AHB1, bit 1 */
#define RCC_GPIOC   0x0002U    /* AHB1, bit 2 */
#define RCC_GPIOD   0x0003U    /* AHB1, bit 3 */

/* ---------- APB1 peripherals (RCC->APB1ENR) ---------- */
#define RCC_TIM2    0x0100U    /* APB1, bit 0 */
#define RCC_TIM3    0x0101U    /* APB1, bit 1 */

/* ---------- APB2 peripherals (RCC->APB2ENR) ---------- */
#define RCC_ADC1    0x0208U    /* APB2, bit 8  */
#define RCC_SYSCFG  0x020EU    /* APB2, bit 14 */

void Rcc_Init(void);
void Rcc_Enable(uint16_t peripheral);

#endif /* RCC_H */
