# State Machine Design

This document details the Finite State Machine (FSM) controlling each elevator.

## States
- **IDLE**: Waiting for a request.
- **MOVING_UP**: Motor running, floor sensors tracking progress.
- **MOVING_DOWN**: Motor running, floor sensors tracking progress.
- **DOORS_OPENING**: Transition state before full access.
- **DOORS_OPEN**: Waiting for passengers (timer based).
- **DOORS_CLOSING**: Transition state before IDLE or MOVING.
- **EMERGENCY_STOP**: Highest priority state, halts all movement.
- **FAULT**: Safety lockout state (e.g., sensor failure).

## Transitions
| From | To | Event |
|---|---|---|
| IDLE | MOVING_UP | Target floor > Current floor |
| IDLE | MOVING_DOWN | Target floor < Current floor |
| MOVING | DOORS_OPENING | Current floor == Target floor |
| DOORS_OPENING | DOORS_OPEN | Operation timer elapsed |
| DOORS_OPEN | DOORS_CLOSING | Open timer elapsed |
| DOORS_CLOSING | IDLE | Operation timer elapsed |
| ANY | EMERGENCY_STOP | E-Stop button press |

## Event Handling
The FSM is event-driven but updated periodically (10ms) by the scheduler. It uses a non-blocking `state_timer` to handle time-based transitions (doors, floor-to-floor travel).
