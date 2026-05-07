Video Link: https://drive.google.com/file/d/1J_jFjnQqdr-vXurJz4BSndE6Y4EAXK65/view?usp=drivesdk

# Project 1 — Embedded Memory & Message Codec Library

A reusable, hardware-agnostic C library for safe memory access and structured message encoding/decoding, designed for simulated embedded communication links.

---

## Table of Contents

- [Project Structure](#project-structure)
- [Part A — Foundation Library](#part-a--foundation-library)
  - [STD_TYPES.h](#std_typesh)
  - [BIT_MATH.h](#bit_mathh)
  - [MemScanner Module](#memscanner-module)
- [Part B — Message Encoder / Decoder](#part-b--message-encoder--decoder)
  - [Message Layout](#message-layout)
  - [MsgCodec Module](#msgcodec-module)
- [Part C — Demonstration](#part-c--demonstration)
- [Build & Run](#build--run)
- [Expected Output](#expected-output)

---

## Project Structure

```
.
├── STD_TYPES.h       # Portable type aliases and return codes
├── BIT_MATH.h        # Bit manipulation macro library
├── MemScanner.h      # Memory scanner public API
├── MemScanner.c      # Memory scanner implementation
├── MsgCodec.h        # Message codec public API, structs, and constants
├── MsgCodec.c        # Message encoder/decoder implementation
└── main.c            # Full demonstration of both modules
```

---

## Part A — Foundation Library

### STD_TYPES.h

Defines all project-wide portable types backed by `<stdint.h>`. No raw C types (`int`, `char`, `long`, `unsigned`) appear anywhere in the project outside this file.

| Type | Underlying Type | Purpose |
|---|---|---|
| `u8` | `uint8_t` | Unsigned 8-bit integer |
| `u16` | `uint16_t` | Unsigned 16-bit integer |
| `u32` | `uint32_t` | Unsigned 32-bit integer |
| `s8` | `int8_t` | Signed 8-bit integer |
| `s16` | `int16_t` | Signed 16-bit integer |
| `s32` | `int32_t` | Signed 32-bit integer |
| `bool_t` | `uint8_t` | Boolean (`TRUE` = 1, `FALSE` = 0) |
| `Std_ReturnType` | `uint8_t` | Function return code (`E_OK` = 0, `E_NOT_OK` = 1) |
| `vptr_t` | `void*` | Generic pointer for raw memory access |

---

### BIT_MATH.h

MISRA-C compliant bit-manipulation macro library. All macros use unsigned literals, are fully parenthesized, and avoid signed-shift operations.

| Macro | Signature | Description |
|---|---|---|
| `SET_BIT` | `(REG, BIT)` | Sets bit `BIT` in `REG` to 1 |
| `CLEAR_BIT` | `(REG, BIT)` | Clears bit `BIT` in `REG` to 0 |
| `TOGGLE_BIT` | `(REG, BIT)` | Flips bit `BIT` in `REG` |
| `READ_BIT` | `(REG, BIT)` | Returns the value (0 or 1) of bit `BIT` in `REG` |
| `SET_MASK` | `(REG, MASK)` | Sets all bits specified by `MASK` in `REG` |
| `CLEAR_MASK` | `(REG, MASK)` | Clears all bits specified by `MASK` in `REG` |
| `WRITE_FIELD` | `(REG, START, WIDTH, VALUE)` | Writes `VALUE` into a bit-field of `REG` starting at bit `START` with `WIDTH` bits |
| `READ_FIELD` | `(REG, START, WIDTH)` | Extracts a `WIDTH`-bit field from `REG` starting at bit `START` |

**Example — WRITE_FIELD / READ_FIELD:**
```c
u8 reg = 0x00u;
WRITE_FIELD(reg, 0u, 3u, 5u);  /* Writes 0b101 into bits [2:0] -> reg = 0x05 */
u8 val = READ_FIELD(reg, 0u, 3u); /* Reads bits [2:0] -> val = 5 */
```

---

### MemScanner Module

A hardware-agnostic memory inspection and manipulation module inspired by real embedded debugger tools such as OpenOCD and pyOCD. All functions accept a `vptr_t` (generic `void*`) base pointer and cast internally.

#### API Reference

```c
u8  MemScanner_ReadByte(vptr_t pBase, u32 offset);
```
Reads and returns the single byte at `pBase + offset`.

---

```c
u16 MemScanner_ReadHalfWord(vptr_t pBase, u32 offset);
```
Reads a 16-bit little-endian value from `pBase + offset`. Reconstructs as:
`(byte[offset]) | (byte[offset+1] << 8)`.

---

```c
u32 MemScanner_ReadWord(vptr_t pBase, u32 offset);
```
Reads a 32-bit little-endian value from `pBase + offset`. Reconstructs across four bytes.

---

```c
void MemScanner_WriteByte(vptr_t pBase, u32 offset, u8 value);
```
Writes `value` into the byte at `pBase + offset`.

---

```c
void MemScanner_HexDump(vptr_t pBase, u32 byteCount);
```
Prints `byteCount` bytes starting at `pBase` in hex format:
```
[HexDump] 0A 1B 2C 3D 4E 5F
```

---

```c
void MemScanner_MemFill(vptr_t pBase, u32 byteCount, u8 fillValue);
```
Fills `byteCount` bytes starting at `pBase` with `fillValue`.

---

```c
u32 MemScanner_MemCompare(vptr_t pBase1, vptr_t pBase2, u32 byteCount);
```
Compares two memory regions byte-by-byte. Returns `0` if identical, or the **1-indexed** offset of the first differing byte.

---

```c
s32 MemScanner_FindByte(vptr_t pBase, u32 byteCount, u8 target);
```
Searches for the first occurrence of `target` in the region. Returns the **0-indexed** offset if found, or `-1` if not found.

---

## Part B — Message Encoder / Decoder

### Message Layout

Every message is exactly **6 bytes**, always fixed.

```
Byte 0 : HEADER   — packed bit-fields (see below)
Byte 1 : LENGTH   — payload size in bytes (always 4)
Bytes 2–5 : PAYLOAD — 4 bytes of message-type-specific data
```

**HEADER byte bit-field map:**

```
Bit  7   6   5   4   |  3  |  2   1   0
    [  SEQ_NUM (4b)  ] [PRI] [ MSG_TYPE (3b) ]
```

| Bits | Field | Description |
|---|---|---|
| 2:0 | `MSG_TYPE` | Message type identifier (0–7) |
| 3 | `PRIORITY` | `0` = Normal, `1` = High |
| 7:4 | `SEQ_NUM` | Auto-incrementing sequence number (0–15, wraps) |

**Message Types:**

| ID | Name | Payload Interpretation |
|---|---|---|
| `0` | `SENSOR_READING` | Lower 16 bits = temperature (×0.1 °C), upper 16 bits = humidity (×0.1 %RH). Accessible as `u32`, `u16[2]`, or `u8[4]`. |
| `1` | `DEVICE_COMMAND` | Byte 0 = Command ID, Byte 1 = Parameter, Bytes 2–3 = Reserved (zero). |

---

### MsgCodec Module

#### Data Structures

```c
/* Wire-format struct — verified at compile time to be exactly 6 bytes */
typedef struct {
    u8 header;
    u8 length;
    u8 payload[4];
} Message_t;

/* Union allowing three views of the 4-byte sensor payload */
typedef union {
    u32 raw32;        /* Full 32-bit value */
    u16 half[2];      /* [0]=temperature, [1]=humidity */
    u8  bytes[4];     /* Individual bytes */
} SensorPayload_t;

/* Decoded result passed to registered handlers */
typedef struct {
    u8      msgType;
    u8      priority;
    u8      seqNum;
    u8      length;
    SensorPayload_t        sensor;   /* valid for MSG_TYPE_SENSOR_READING */
    DeviceCommandPayload_t command;  /* valid for MSG_TYPE_DEVICE_COMMAND  */
} DecodedMessage_t;

/* Handler function pointer */
typedef void (*MsgHandler_t)(const DecodedMessage_t *pMsg);
```

#### API Reference

```c
Std_ReturnType MsgCodec_RegisterHandler(u8 msgType, MsgHandler_t handler);
```
Registers a callback function for a given message type (0–7). Pass `NULL` to deregister. Returns `E_NOT_OK` if `msgType >= MSG_TYPE_COUNT`. The handler table is `static` and not accessible outside `MsgCodec.c`.

---

```c
Std_ReturnType MsgCodec_Encode(u8 msgType, u8 priority,
                               const u8 *pPayload, u8 *pOutBuf);
```
Encodes a message into the 6-byte buffer at `pOutBuf`:
- Packs `msgType`, `priority`, and the internal sequence number into the header byte using `WRITE_FIELD`.
- Copies the 4 payload bytes.
- Auto-increments the internal sequence number (wraps at 4-bit boundary, i.e., 0–15). The sequence number state is `static` and invisible to callers.
- Returns `E_NOT_OK` if any pointer is `NULL` or `msgType` is out of range.

---

```c
Std_ReturnType MsgCodec_Decode(vptr_t pRawBuf, DecodedMessage_t *pResult);
```
Decodes a raw 6-byte buffer into a `DecodedMessage_t`:
- Extracts `msgType`, `priority`, and `seqNum` from the header using `READ_FIELD` / `READ_BIT`.
- Populates the appropriate payload view (`sensor` or `command`) based on message type.
- Automatically dispatches to the registered handler for `msgType` (if one exists).
- Returns `E_NOT_OK` if any pointer is `NULL` or the decoded type is out of range.

---

## Part C — Demonstration

`main.c` demonstrates the complete library in sequence:

| Step | What it shows |
|---|---|
| C1-1 | Register `SensorReadingHandler` and `DeviceCommandHandler` |
| C1-2 | Encode a `SENSOR_READING` message (25.5 °C, 60.0 %RH) |
| C1-3 | Encode a `DEVICE_COMMAND` message (cmdId=0x01, param=0xFF) |
| C1-4/5 | Decode both messages; handlers print all field views |
| C2-6 | `HexDump` of both encoded message buffers |
| C2-7 | `ReadByte` for HEADER byte (offset 0) and LENGTH byte (offset 1) |
| C2-8 | `ReadHalfWord` for temperature from payload offset 2 |
| C2-9 | `ReadWord` for full 32-bit payload from offset 2 |
| C2-10 | `WriteByte` to toggle PRIORITY bit in a buffer copy; print both with HexDump |
| C2-11 | `MemFill` an empty buffer with `0xAA`; verify with HexDump |
| C2-12 | `MemCompare` identical buffers (expect 0), then after modifying one byte (expect 3) |
| C2-13 | `FindByte` for a known payload byte (returns offset); then for `0xDE` (returns -1) |

---

## Build & Run

Compile with any C99-compatible compiler:

```bash
gcc -std=c99 -Wall -Wextra -o project1 main.c MsgCodec.c MemScanner.c
./project1
```

No external libraries or platform-specific headers are required beyond the standard C library (`<stdio.h>`, `<stdint.h>`, `<string.h>`).

---

## Expected Output

```
=== Part C: Demonstration ===

--- C1: Message Encode / Decode ---
[INFO] Handlers registered for SENSOR_READING and DEVICE_COMMAND.

[ENCODE] SENSOR_READING  ret=E_OK
  Payload: temp=25.5 degC (255 units)  hum=60.0 %RH (600 units)

[ENCODE] DEVICE_COMMAND  ret=E_OK
  Payload: cmdId=0x01  param=0xFF

--- Decoding messages (handlers will print) ---
[HANDLER] SENSOR_READING received:
  SEQ=0   PRIORITY=NORMAL
  Raw 32-bit value : 0x02580FF  (...)
  Temperature (u16): 0x00FF  -> 25.5 deg C
  Humidity    (u16): 0x0258  -> 60.0 %RH
  Individual bytes : [0]=0xFF [1]=0x00 [2]=0x58 [3]=0x02

[HANDLER] DEVICE_COMMAND received:
  SEQ=1   PRIORITY=HIGH
  Command ID : 0x01
  Parameter  : 0xFF

--- C2: Memory Scanner Demonstration ---
[C2-6] HexDump of sensorBuf : [HexDump] 00 04 FF 00 58 02
[C2-6] HexDump of cmdBuf    : [HexDump] 18 04 01 FF 00 00
...
=== Demonstration complete ===
```

---

## Design Notes

- **No raw C types** anywhere outside `STD_TYPES.h` — all code uses the project's type aliases.
- **`static` encapsulation** — the sequence number counter and handler table in `MsgCodec.c` are module-private and not accessible via any public API.
- **Compile-time size guard** — `typedef u8 _assert_msg_size[(sizeof(Message_t) == 6u) ? 1 : -1];` will cause a compilation error if `Message_t` is not exactly 6 bytes.
- **Little-Endian assumption** — `MemScanner_ReadHalfWord` and `MemScanner_ReadWord` explicitly reconstruct values from bytes in little-endian order via byte-by-byte reads, making the behavior portable regardless of host endianness.
- **Union-based payload view** — `SensorPayload_t` uses a union to provide three simultaneous views of the same 4 bytes with zero copying or conversion overhead.
