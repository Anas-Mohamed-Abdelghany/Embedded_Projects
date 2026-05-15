# Scheduler Design

This document describes the cooperative scheduler used in the dual-elevator firmware.

## Architecture
The scheduler is a tick-based cooperative system running in the main loop. It uses a hardware timer (TIM2) to generate a 1ms base tick.

## Interrupt Flow
1. **TIM2 ISR**: Triggers every 1ms.
2. **Scheduler_Tick()**: Increments `system_tick` and marks tasks as `ready` if their interval has elapsed.
3. **Main Loop**: Continuously calls `Scheduler_Run()`.
4. **Scheduler_Run()**: Executes all `ready` tasks in sequence and resets their state.

## Task Timing
- **Elevator FSM (10ms)**: High-frequency logic update for smooth transitions.
- **Master/Slave IPC (50ms)**: Periodic SPI sync between microcontrollers.
- **Hallway/Cabin Input (20ms)**: Debounced input processing.
- **Telemetry (500ms)**: Non-blocking UART output.

## Safety
Shared variables (tick, flags) are protected using `Enter_Critical()` and `Exit_Critical()` blocks to prevent race conditions during timer interrupts.
