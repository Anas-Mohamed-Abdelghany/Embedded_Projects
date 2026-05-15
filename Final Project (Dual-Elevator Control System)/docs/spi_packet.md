# SPI Packet Protocol

This document defines the 8-byte SPI frame used for Inter-Processor Communication (IPC) between Master and Slave microcontrollers.

## Frame Structure

| Byte | Field | Description |
|---|---|---|
| 0 | Header | Constant value `0xA5` |
| 1 | Elevator ID | 1 for Master (Elev A), 2 for Slave (Elev B) |
| 2 | Current Floor | Values 1 to 4 |
| 3 | Target Floor | Values 1 to 4 |
| 4 | Elevator State | Enum value (IDLE, MOVING, etc.) |
| 5 | Request Bitmap | Bitfield for floor requests |
| 6 | Status Flags | Faults, sensors, etc. |
| 7 | Checksum | Sum of bytes 0 to 6 |

## Checksum Logic
The checksum is a simple 8-bit wrap-around sum of the first 7 bytes.
```c
uint8_t sum = 0;
for (int i = 0; i < 7; i++) {
    sum += packet[i];
}
packet[7] = sum;
```

## Communication Flow
- **Interval**: 50 ms
- **Mode**: Full Duplex SPI
- **Master**: Initiates transfer by sending its own status and receiving Slave status.
- **Slave**: Preloads the transmit buffer with current status before Master starts the clock.
