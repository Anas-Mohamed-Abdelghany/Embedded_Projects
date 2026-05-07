#ifndef MEMSCANNER_H
#define MEMSCANNER_H

#include "STD_TYPES.h"

/*
 * MemScanner — hardware-agnostic memory inspection and manipulation module.
 * Inspired by embedded debugger tools such as OpenOCD and pyOCD.
 */

/* Read a single byte from a given memory address at the specified byte offset. */
u8  MemScanner_ReadByte    (vptr_t pBase, u32 offset);

/* Read a 16-bit (half-word) value from a memory address at a given byte offset (Little-Endian). */
u16 MemScanner_ReadHalfWord(vptr_t pBase, u32 offset);

/* Read a 32-bit (word) value from a memory address at a given byte offset (Little-Endian). */
u32 MemScanner_ReadWord    (vptr_t pBase, u32 offset);

/* Write a single byte value into a memory region at the given offset. */
void MemScanner_WriteByte  (vptr_t pBase, u32 offset, u8 value);

/* Print a region of memory as formatted hex bytes. */
void MemScanner_HexDump    (vptr_t pBase, u32 byteCount);

/* Fill a memory region of a given size with a specified byte value. */
void MemScanner_MemFill    (vptr_t pBase, u32 byteCount, u8 fillValue);

/*
 * Compare two memory regions byte-by-byte.
 * Returns 0 if identical, or the 1-indexed offset of the first differing byte.
 */
u32 MemScanner_MemCompare  (vptr_t pBase1, vptr_t pBase2, u32 byteCount);

/*
 * Search a memory region for the first occurrence of a specified byte value.
 * Returns the offset if found, or -1 (cast to s32) if not found.
 */
s32 MemScanner_FindByte    (vptr_t pBase, u32 byteCount, u8 target);

#endif /* MEMSCANNER_H */
