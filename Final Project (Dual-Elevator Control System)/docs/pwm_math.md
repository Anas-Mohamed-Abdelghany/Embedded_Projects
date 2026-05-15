# PWM Math - Motor Simulation

This document explains the calculations for the 10 kHz PWM signal used to simulate motor speed.

## Parameters
- **Clock Source (HSI)**: 16 MHz
- **Target Frequency**: 10 kHz
- **Timer**: TIM3

## Formula
$$Frequency = \frac{f_{clk}}{(PSC + 1) \times (ARR + 1)}$$

## Calculations
To achieve 10 kHz:
$$10,000 = \frac{16,000,000}{(PSC + 1) \times (ARR + 1)}$$

Let $PSC = 3$ (divide by 4):
$$10,000 = \frac{16,000,000}{4 \times (ARR + 1)}$$
$$ARR + 1 = 400$$
$$ARR = 399$$

## Resulting Configuration
- **PSC**: 3
- **ARR**: 399
- **Duty Cycle**:
  - 0%   -> CCR3 = 0
  - 20%  -> CCR3 = 80
  - 100% -> CCR3 = 400
