# 🤖 Bare-Metal Embedded C Showcase

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-A8B9CC?style=for-the-badge&logo=c" alt="Language: C">
  <img src="https://img.shields.io/badge/Architecture-ARM%20Cortex--M-orange?style=for-the-badge" alt="Architecture: ARM">
  <img src="https://img.shields.io/badge/MCU-STM32F4-blue?style=for-the-badge&logo=stmicroelectronics" alt="MCU: STM32F4">
  <img src="https://img.shields.io/badge/Style-Bare--Metal-red?style=for-the-badge" alt="Style: Bare-Metal">
  <img src="https://img.shields.io/badge/License-MIT-green?style=for-the-badge" alt="License: MIT">
</p>

Welcome to my collection of embedded C projects, written entirely from scratch without vendor-provided HALs, CMSIS, or high-level libraries. This repository demonstrates core concepts in embedded systems programming, from low-level memory manipulation to complex, state-driven applications on ARM Cortex-M microcontrollers.

The philosophy behind these projects is to build robust, modular, and efficient firmware by directly manipulating hardware registers and understanding the underlying architecture.

---

## 📂 Projects in this Collection

This repository is organized into three distinct projects, each in its own sub-directory.

### 1. [Embedded Memory & Message Codec Library](./Project%201%20(Memory%20&%20Codec%20Library)/)

A reusable, hardware-agnostic C library for safe memory access and structured message encoding/decoding. This project serves as a foundational toolkit for building reliable communication protocols in memory-constrained environments.

**🚀 Key Features:**
*   Portable, MISRA-C compliant bit-math macros.
*   Hardware-agnostic memory scanner for reading, writing, and comparing memory regions.
*   A fixed-size message codec with bit-field packing/unpacking.
*   Callback-based dispatcher for handling different message types.

**💡 Core Concepts Demonstrated:**
*   Data Abstraction & Portability (`STD_TYPES.h`).
*   Bit Manipulation and Bit Fields.
*   Pointer Arithmetic and Safe Memory Access.
*   Union-based data views for zero-overhead type punning.
*   Static encapsulation for module-private state.

### 2. [STM32 Bare-Metal Door Lock System](./Project%202%20(Door%20Lock%20System)/)

A complete bare-metal security system for an STM32F4 microcontroller. It implements a PIN-based door lock using a keypad, 7-segment display, and LEDs, all managed by a robust Mealy finite state machine.

**🚀 Key Features:**
*   Direct register access for GPIO, RCC, and SysTick peripherals.
*   Interrupt-driven input from push-buttons using EXTI.
*   Polled, debounced input from a 4x3 matrix keypad.
*   Finite State Machine (FSM) to manage system states (LOCKED, UNLOCKED, ALARM).
*   Non-blocking timers for effects like LED pulses.

**💡 Core Concepts Demonstrated:**
*   Bare-metal driver development (GPIO, RCC, EXTI, SysTick).
*   Interrupt Service Routines (ISRs) and the Nested Vectored Interrupt Controller (NVIC).
*   Mixing polled and interrupt-driven I/O.
*   State machine design for event-driven systems.
*   Critical sections for safe state transitions.

### 3. [STM32 Auto-Cooler System](./Project%203%20(Auto-Cooler%20System)/)

A real-time temperature control system for the STM32F401. It uses an ADC to read an LM35 temperature sensor and dynamically controls a cooling fan via PWM, all driven by a state machine and displayed on a 16x2 LCD.

**🚀 Key Features:**
*   Interrupt-driven ADC sampling for efficient, non-blocking sensor reads.
*   Timer-based PWM generation (TIM3) for variable fan speed control.
*   Modular drivers for ADC, PWM, Timers, and a 16x2 LCD.
*   Mealy FSM that adapts fan speed based on real-time temperature gradients.
*   Overheat protection with a visual alarm.

**💡 Core Concepts Demonstrated:**
*   Analog-to-Digital Conversion (ADC) with End-of-Conversion interrupts.
*   Pulse Width Modulation (PWM) for motor control.
*   Writing parallel-interface peripheral drivers (HD44780 LCD).
*   Real-time control loops.
*   Building a complete, sensor-driven feedback system.

---

## 🛠️ Building & Toolchain

All projects are written in standard C99 and designed to be compiled with the **ARM GCC toolchain**.

**Required Tools:**
*   `arm-none-eabi-gcc`
*   `make` or `cmake`
*   (For STM32 projects) An ST-Link programmer/debugger and `st-flash` utility.

Each project directory contains specific instructions in its own `README.md` for building and running the code. A typical compilation command looks like this:

```bash
# Navigate to a specific project directory
cd ./project-2-door-lock/

# Compile using the ARM GCC toolchain
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 src/*.c -I include/ -T linker.ld -o output.elf

# Flash to the target device
st-flash write output.bin 0x08000000
```

---

## 📜 License

This entire collection is licensed under the **MIT License**. See the `LICENSE` file for more details. You are free to use, modify, and learn from this code.
