# STM32 Bare-Metal Door Lock System

A bare-metal embedded security system built for the STM32F4 microcontroller, written entirely without HAL or CMSIS drivers. It implements a PIN-based door lock with a keypad, 7-segment display, LED feedback, doorbell, and interrupt-driven buttons — all managed by a finite state machine.

---

## Table of Contents

- [Features](#features)
- [Hardware Setup](#hardware-setup)
- [Pin Definitions](#pin-definitions)
- [System Architecture](#system-architecture)
- [Module Descriptions](#module-descriptions)
- [State Machine](#state-machine)
- [Configuration](#configuration)
- [Building & Flashing](#building--flashing)

---

## Features

- 4-digit PIN entry via a 4×3 matrix keypad
- 7-segment display showing incorrect attempt count
- 4-LED progress bar tracking correct key presses
- Status LEDs for success, alarm, and doorbell
- Non-blocking doorbell LED pulse (300 ms)
- Interrupt-driven buttons for reset, doorbell, and lock
- Auto-lockout after 3 failed attempts
- Mealy finite state machine for robust state management
- SysTick-based millisecond timebase (no HAL)

---

## Hardware Setup

| Component | Interface |
|---|---|
| STM32F4 (16 MHz internal clock) | — |
| 4×3 Matrix Keypad | Port A, Pins 0–6 |
| 7-Segment Display (Common Anode) | Port B, Pins 0–6 |
| LED Progress Bar | Port B, Pins 8–11 |
| Status LEDs (Success / Alarm / Bell) | Port D, Pins 0–2 |
| Push Buttons (Reset / Bell / Lock) | Port E, Pins 0–2 |

---

## Pin Definitions

All pin assignments are defined in `main.h`.

### Keypad (Port A)

| Signal | Pin |
|---|---|
| Row 0 | PA0 |
| Row 1 | PA1 |
| Row 2 | PA2 |
| Row 3 | PA3 |
| Col 0 | PA4 |
| Col 1 | PA5 |
| Col 2 | PA6 |

### 7-Segment Display — Segments A–G (Port B)

| Segment | Pin |
|---|---|
| A | PB0 |
| B | PB1 |
| C | PB2 |
| D | PB3 |
| E | PB4 |
| F | PB5 |
| G | PB6 |

### LED Progress Bar (Port C)

| LED | Pin |
|---|---|
| LED 1 | PC0 |
| LED 2 | PC1 |
| LED 3 | PC2 |
| LED 4 | PC3 |

### Status LEDs (Port D)

| LED | Pin |
|---|---|
| Success (Green) | PD0 |
| Alarm (Red) | PD1 |
| Bell | PD2 |

### Buttons (Port E — EXTI, Falling Edge)

| Button | Pin |
|---|---|
| Emergency Reset | PE0 |
| Doorbell | PE1 |
| Lock | PE2 |

---

## System Architecture

```
main.c
 ├── SysTick (1 ms timebase)
 ├── Keypad_Update()  ← polls keypad, sends SM events
 ├── LED_BellTick()   ← non-blocking bell timer
 └── delay_ms()

state_machine.c   ← Mealy FSM, owns all state transitions
display.c         ← 7-segment, LED bar, status LEDs
keypad.c          ← matrix scan, debounce, sequence validation
exti.c            ← EXTI driver, ISR handlers, callback dispatch
gpio.c            ← register-level GPIO read/write/init
rcc.c             ← AHB1ENR clock enable per port
```

---

## Module Descriptions

### `main.c` / `main.h`
Entry point. Initialises SysTick at 1 ms intervals using the 16 MHz internal clock (`Reload = 16000 - 1`). Calls peripheral and state machine init, then runs the super-loop: poll keypad → tick bell timer → 10 ms delay.

### `rcc.c` / `rcc.h`
Enables GPIO port clocks via `RCC_AHB1ENR` (base `0x40023800 + 0x30`). Call `Rcc_Enable('A'…'E')` before using any pin on that port.

### `gpio.c` / `gpio.h`
Register-level GPIO driver. Uses a `GpioType` struct overlay at the port base addresses.

| Function | Description |
|---|---|
| `Gpio_Init(port, pin, mode, default)` | Sets MODER, enables pull-up for inputs, writes initial state for outputs |
| `Gpio_WritePin(port, pin, data)` | Writes via ODR |
| `Gpio_ReadPin(port, pin)` | Reads via IDR |

Modes: `GPIO_INPUT (0x00)`, `GPIO_OUTPUT (0x01)`.

### `display.c` / `display.h`
Controls all visual output.

| Function | Description |
|---|---|
| `Display_Init()` | Enables clocks for Port B and D, configures all output pins |
| `SevenSeg_ShowDigit(digit)` | Drives PB0–PB6 using a segment lookup table (Common Anode: LOW = ON) |
| `SevenSeg_Clear()` | Sets all segment pins HIGH (all OFF) |
| `LEDBar_SetProgress(count)` | Lights 0–4 LEDs on PB8–PB11 |
| `LED_SetSuccess(on)` | Controls PD0 |
| `LED_SetAlarm(on)` | Controls PD1 |
| `LED_PulseBell()` | Turns PD2 ON and records the start tick |
| `LED_BellTick()` | Called from main loop; turns PD2 OFF after `BELL_PULSE_MS` |

### `keypad.c` / `keypad.h`
Drives a 4×3 matrix keypad on Port A.

- **Scan**: Pulls each row LOW in turn, reads columns, debounces with a 30 ms re-check.
- **Update**: Only active in `STATE_LOCKED`. Tracks total keys pressed and correct keys per attempt. After `PASSWORD_LENGTH` keys, fires `EVENT_SEQUENCE_COMPLETE` or `EVENT_INVALID_KEY`. Correct individual keys fire `EVENT_VALID_KEY` for progress bar feedback.

### `exti.c` / `exti.h`
Interrupt driver for the three push buttons.

| Function | Description |
|---|---|
| `Interrupts_Init()` | Enables SYSCFG clock, configures PE0–PE2 as inputs, registers callbacks |
| `Exti_EnableInterrupt(line, port, edge, cb)` | Routes port to EXTI line via SYSCFG, sets edge, unmasks in EXTI IMR, enables in NVIC |
| `Exti_Disable(line)` / `Exti_Enable(line)` | Used for critical sections in the state machine |

Callbacks registered: `Handle_Reset_Button` → `EVENT_EMERGENCY_RESET`, `Handle_Bell_Button` → `EVENT_DOORBELL`, `Handle_Lock_Button` → `EVENT_LOCK_CMD`.

> **NVIC mapping used:** EXTI line N → IRQ N+6 (lines 0–4 → IRQs 6–10).

### `state_machine.c` / `state_machine.h`
Mealy FSM with three states and seven events. Uses a critical section (disable/re-enable EXTI interrupts) around every `SM_ProcessEvent()` call to prevent button ISRs from corrupting shared state mid-transition.

---

## State Machine

### States

| State | Description |
|---|---|
| `STATE_LOCKED` | Waiting for correct PIN entry |
| `STATE_UNLOCKED` | Access granted; waiting for lock command |
| `STATE_ALARM` | Lockout active after too many failures |

### Events

| Event | Source |
|---|---|
| `EVENT_VALID_KEY` | Keypad — correct key in sequence |
| `EVENT_SEQUENCE_COMPLETE` | Keypad — full correct PIN entered |
| `EVENT_INVALID_KEY` | Keypad — wrong PIN attempt |
| `EVENT_LOCK_CMD` | Button interrupt (PE2) |
| `EVENT_DOORBELL` | Button interrupt (PE1) |
| `EVENT_EMERGENCY_RESET` | Button interrupt (PE0) |

### Transition Table

| Current State | Event | Next State | Output |
|---|---|---|---|
| LOCKED | VALID_KEY | LOCKED | Progress bar +1 |
| LOCKED | SEQUENCE_COMPLETE | UNLOCKED | Progress bar clear, success LED ON |
| LOCKED | INVALID_KEY | LOCKED / ALARM | Failure count +1, 7-seg update; ALARM if ≥ threshold |
| LOCKED | DOORBELL | LOCKED | Bell pulse |
| LOCKED | EMERGENCY_RESET | LOCKED | Reset all counters and displays |
| UNLOCKED | LOCK_CMD | LOCKED | Success LED OFF |
| UNLOCKED | DOORBELL | UNLOCKED | Bell pulse |
| ALARM | EMERGENCY_RESET | LOCKED | Alarm LED OFF, reset all |
| ALARM | DOORBELL | ALARM | Bell pulse |

---

## Configuration

All tunable constants live in `main.h`:

| Constant | Default | Description |
|---|---|---|
| `PASSWORD_LENGTH` | `4` | Number of digits in the PIN |
| `LOCKOUT_THRESHOLD` | `3` | Failed attempts before alarm |
| `BELL_PULSE_MS` | `300` | Doorbell LED on-time in milliseconds |
| `SECRET_CODE` | `{'1','2','3','4'}` | The PIN — change before deployment |

---

## Building & Flashing

This project has no HAL dependency. Use any ARM GCC toolchain targeting Cortex-M4.

```bash
# Example with arm-none-eabi-gcc
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O1 \
  src/*.c -I include/ \
  -T linker.ld -o lock_system.elf

arm-none-eabi-objcopy -O binary lock_system.elf lock_system.bin

# Flash with ST-Link
st-flash write lock_system.bin 0x08000000
```

>  linker script places the vector table at `0x08000000` and maps the SysTick handler correctly.

---

## License

MIT — free to use, modify, and distribute.