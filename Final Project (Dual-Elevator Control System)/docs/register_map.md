# Register Map - STM32F401VE

This document lists the primary registers used in the dual-elevator system.

## Reset and Clock Control (RCC)
- **RCC_AHB1ENR**: Enable GPIOA, GPIOB, GPIOC clocks.
- **RCC_APB1ENR**: Enable TIM2, TIM3 clocks.
- **RCC_APB2ENR**: Enable SPI1, USART1, SYSCFG clocks.

## GPIO Registers
- **MODER**: Mode selection (Input, Output, AF, Analog).
- **PUPDR**: Pull-up/Pull-down selection.
- **AFR[0-1]**: Alternate Function mapping.
- **BSRR**: Bit Set/Reset register for atomic pin writing.
- **IDR**: Input Data Register.

## Timers (TIM2, TIM3)
- **PSC**: Prescaler (determines clock frequency).
- **ARR**: Auto-reload Register (determines period/frequency).
- **CCR3**: Capture/Compare Register (determines PWM duty cycle on TIM3).
- **DIER**: DMA/Interrupt Enable Register.
- **SR**: Status Register.

## SPI1 IPC
- **CR1**: Control Register 1 (Master/Slave, Baud rate, Mode).
- **CR2**: Control Register 2 (Interrupt enables).
- **SR**: Status Register (TXE, RXNE flags).
- **DR**: Data Register.

## USART1 Telemetry
- **BRR**: Baud Rate Register (115200 at 16MHz).
- **CR1**: Control Register 1 (TE, RE, UE, Interrupts).
- **SR**: Status Register (TXE flag).
- **DR**: Data Register.

## EXTI & SYSCFG
- **SYSCFG_EXTICR**: Mapping GPIO pins to EXTI lines.
- **EXTI_IMR**: Interrupt Mask Register.
- **EXTI_RTSR/FTSR**: Rising/Falling Trigger selection.
- **EXTI_PR**: Pending Register.
