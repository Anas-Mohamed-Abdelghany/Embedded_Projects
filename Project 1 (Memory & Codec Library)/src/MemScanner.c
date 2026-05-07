#include <stdio.h>
#include "MemScanner.h"

u8 MemScanner_ReadByte(vptr_t pBase, u32 offset)
{
    const u8 *pMem = (const u8 *)pBase;
    return pMem[offset];
}

u16 MemScanner_ReadHalfWord(vptr_t pBase, u32 offset)
{
    const u8 *pMem = (const u8 *)pBase;
    /* Little-Endian: lower address holds the least significant byte */
    return (u16)((u16)pMem[offset] | ((u16)pMem[offset + 1u] << 8u));
}

u32 MemScanner_ReadWord(vptr_t pBase, u32 offset)
{
    const u8 *pMem = (const u8 *)pBase;
    /* Little-Endian: reconstruct 32-bit value from four bytes */
    return ((u32)pMem[offset]         |
            ((u32)pMem[offset + 1u] << 8u)  |
            ((u32)pMem[offset + 2u] << 16u) |
            ((u32)pMem[offset + 3u] << 24u));
}

void MemScanner_WriteByte(vptr_t pBase, u32 offset, u8 value)
{
    u8 *pMem = (u8 *)pBase;
    pMem[offset] = value;
}

void MemScanner_HexDump(vptr_t pBase, u32 byteCount)
{
    const u8 *pMem = (const u8 *)pBase;
    u32 i;

    printf("[HexDump]");
    for (i = 0u; i < byteCount; i++)
    {
        printf(" %02X", pMem[i]);
    }
    printf("\n");
}

void MemScanner_MemFill(vptr_t pBase, u32 byteCount, u8 fillValue)
{
    u8  *pMem = (u8 *)pBase;
    u32  i;

    for (i = 0u; i < byteCount; i++)
    {
        pMem[i] = fillValue;
    }
}

u32 MemScanner_MemCompare(vptr_t pBase1, vptr_t pBase2, u32 byteCount)
{
    const u8 *pA = (const u8 *)pBase1;
    const u8 *pB = (const u8 *)pBase2;
    u32        i;

    for (i = 0u; i < byteCount; i++)
    {
        if (pA[i] != pB[i])
        {
            return i + 1u; /* 1-indexed offset of first difference */
        }
    }
    return 0u; /* identical */
}

s32 MemScanner_FindByte(vptr_t pBase, u32 byteCount, u8 target)
{
    const u8 *pMem = (const u8 *)pBase;
    u32        i;

    for (i = 0u; i < byteCount; i++)
    {
        if (pMem[i] == target)
        {
            return (s32)i;
        }
    }
    return -1; /* not found */
}
