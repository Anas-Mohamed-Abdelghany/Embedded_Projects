# 🚀 Dual-Elevator Control System (STM32 Bare-Metal)

![STM32](https://img.shields.io/badge/MCU-STM32F401VE-blue?style=for-the-badge&logo=stmicroelectronics)
![Language](https://img.shields.io/badge/Language-C11-green?style=for-the-badge&logo=c)
![Build](https://img.shields.io/badge/Build-CMake-red?style=for-the-badge&logo=cmake)
![Architecture](https://img.shields.io/badge/Architecture-Bare--Metal-orange?style=for-the-badge)

A production-grade, modular firmware for a collaborative dual-elevator system. This project implements a fully bare-metal architecture, interacting directly with MCU registers to manage complex elevator logic, inter-processor communication (IPC), and real-time task scheduling.

<img width="960" height="509" alt="Screenshot 2026-05-15 211151" src="https://github.com/user-attachments/assets/c704a3fe-04ec-49ca-a5f7-04e60bf74278" />

---

## 🛠 Project Highlights

- **Pure Bare-Metal Implementation:** Zero dependency on CMSIS or HAL headers. All peripheral registers are manually mapped for maximum performance and educational depth.
- **Inter-Processor Communication (IPC):** Non-blocking full-duplex SPI link for coordination between the Master and Slave MCUs.
- **Event-Driven Finite State Machine (FSM):** Robust state management (IDLE, MOVING, DOORS, EMERGENCY) with asynchronous request handling.
- **Custom Lightweight Utilities:** Includes a hand-crafted `mini_snprintf` to eliminate standard library overhead and `nosys.specs` dependencies.
- **Cooperative Scheduler:** Custom tick-based scheduler (using TIM2) for non-blocking execution of diagnostics, debouncing, and FSM updates.
- **Proteus Optimized:** Built-in support for generating HEX files compatible with Proteus simulation environments.

---

## 🏗 System Architecture

The system is split into two primary controllers communicating over SPI:
- **Master MCU:** Handles global task allocation, system-wide diagnostics, and primary elevator logic.
- **Slave MCU:** Manages secondary elevator operations and synchronizes state with the Master.

### Layered Structure
- **Hardware Abstraction (HAL-ish):** `gpio`, `rcc`, `spi`, `uart`, `timer`, `pwm`, `exti`.
- **System Services:** `scheduler`, `debounce`, `queue`, `communication`.
- **Application Logic:** `state_machine` (FSM), `diagnostics`, `emergency`.

---

## 📂 Project Structure

```text
├── communication/      # SPI IPC protocol and packet handling
├── debounce/           # Software button debouncing logic
├── diagnostics/        # Real-time system health monitoring
├── emergency/          # Priority emergency stop handling
├── exti/               # External interrupt configurations
├── gpio/               # Low-level port/pin management
├── interrupts/         # NVIC and ISR management
├── main/               # Entry point and MCU role selection
├── pwm/                # Motor/Indicator control (Timer-based)
├── queue/              # Floor request management
├── rcc/                # Clock tree configuration
├── scheduler/          # Task scheduling (TIM2 driven)
├── spi/                # SPI peripheral driver (Register-level)
├── state_machine/      # Elevator FSM and Dispatcher
├── timer/              # General purpose timer management
├── uart/               # Serial debugging and telemetry
├── STM32F401xE.ld      # Custom linker script
└── CMakeLists.txt      # Project build configuration
```

---

## 🚀 Getting Started

### Prerequisites
- **Compiler:** `arm-none-eabi-gcc`
- **Build System:** `CMake` (>= 3.10)
- **Toolchain:** ARM GNU Toolchain

### Building
1. Clone the repository.
2. Initialize the build directory:
   ```powershell
   mkdir build
   cd build
   cmake ..
   ```
3. Compile the project:
   ```powershell
   make -j4
   ```
   *The build will generate `ElevatorSystem.elf`, `ElevatorSystem.map`, and `ElevatorSystem.hex`.*

---

## ⚙️ Configuration

The MCU role (Master/Slave) is defined in `main/main.h`:

```c
/* Select MCU Role */
#define MASTER_MCU   // Uncomment for Master
//#define SLAVE_MCU  // Uncomment for Slave
```

---

## 📊 System Logic

### Elevator States
- `STATE_IDLE`: Waiting for requests.
- `STATE_MOVING_UP/DOWN`: Active transit between floors.
- `STATE_DOORS_OPENING/OPEN/CLOSING`: Passenger entry/exit sequence.
- `STATE_EMERGENCY_STOP`: Critical halt triggered by safety sensors.
- `STATE_FAULT`: Hardware or logic error detected.

### IPC Packet Format
The system uses a 8-byte packet for high-speed synchronization:
- `Header (0xA5)`
- `Elevator ID`
- `Current/Target Floor`
- `State Bitmap`
- `Checksum (XOR based)`

---

## 📜 License
This project is developed as part of the **Embedded Systems Final Project**. All rights reserved.
