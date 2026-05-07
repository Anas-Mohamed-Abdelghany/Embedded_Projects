#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "MemScanner.h"
#include "MsgCodec.h"

/* =========================================================================
 * Message handler callbacks (Part B4)
 * ========================================================================= */

static void SensorReadingHandler(const DecodedMessage_t *pMsg)
{
    printf("\n[HANDLER] SENSOR_READING received:\n");
    printf("  SEQ=%-2u  PRIORITY=%s\n",
           pMsg->seqNum,
           (pMsg->priority == MSG_PRIORITY_HIGH) ? "HIGH" : "NORMAL");
    printf("  Raw 32-bit value : 0x%08lX  (%lu)\n",
           (unsigned long)pMsg->sensor.raw32,
           (unsigned long)pMsg->sensor.raw32);
    printf("  Temperature (u16): 0x%04X  -> %.1f deg C\n",
           pMsg->sensor.half[0u],
           (double)pMsg->sensor.half[0u] / 10.0);
    printf("  Humidity    (u16): 0x%04X  -> %.1f %%RH\n",
           pMsg->sensor.half[1u],
           (double)pMsg->sensor.half[1u] / 10.0);
    printf("  Individual bytes : [0]=0x%02X [1]=0x%02X [2]=0x%02X [3]=0x%02X\n",
           pMsg->sensor.bytes[0u], pMsg->sensor.bytes[1u],
           pMsg->sensor.bytes[2u], pMsg->sensor.bytes[3u]);
}

static void DeviceCommandHandler(const DecodedMessage_t *pMsg)
{
    printf("\n[HANDLER] DEVICE_COMMAND received:\n");
    printf("  SEQ=%-2u  PRIORITY=%s\n",
           pMsg->seqNum,
           (pMsg->priority == MSG_PRIORITY_HIGH) ? "HIGH" : "NORMAL");
    printf("  Command ID : 0x%02X\n", pMsg->command.cmdId);
    printf("  Parameter  : 0x%02X\n", pMsg->command.param);
}

/* =========================================================================
 * Helpers
 * ========================================================================= */

/* Build a 4-byte sensor payload from temperature (0.1 °C) and humidity (0.1 %RH) */
static void BuildSensorPayload(u16 tempTenths, u16 humTenths, u8 *pPayload)
{
    /* Little-Endian packing: temperature in lower 16 bits, humidity in upper 16 bits */
    pPayload[0u] = (u8)(tempTenths & 0xFFu);
    pPayload[1u] = (u8)((tempTenths >> 8u) & 0xFFu);
    pPayload[2u] = (u8)(humTenths & 0xFFu);
    pPayload[3u] = (u8)((humTenths >> 8u) & 0xFFu);
}

/* Build a 4-byte device-command payload */
static void BuildCommandPayload(u8 cmdId, u8 param, u8 *pPayload)
{
    pPayload[0u] = cmdId;
    pPayload[1u] = param;
    pPayload[2u] = 0x00u;
    pPayload[3u] = 0x00u;
}

/* =========================================================================
 * main
 * ========================================================================= */
int main(void)
{
    u8  sensorBuf[MSG_SIZE];
    u8  cmdBuf[MSG_SIZE];
    u8  sensorBufCopy[MSG_SIZE];
    u8  sensorBuf2[MSG_SIZE];
    u8  emptyBuf[MSG_SIZE];
    u8  payload[MSG_PAYLOAD_SIZE];

    DecodedMessage_t decoded;
    Std_ReturnType   ret;
    u32              cmpResult;
    s32              findResult;
    u8               byteVal;
    u16              halfVal;
    u32              wordVal;

    /* -----------------------------------------------------------------------
     * C1-1: Register handlers
     * --------------------------------------------------------------------- */
    printf("=== Part C: Demonstration ===\n");
    printf("\n--- C1: Message Encode / Decode ---\n");

    MsgCodec_RegisterHandler(MSG_TYPE_SENSOR_READING, SensorReadingHandler);
    MsgCodec_RegisterHandler(MSG_TYPE_DEVICE_COMMAND, DeviceCommandHandler);
    printf("[INFO] Handlers registered for SENSOR_READING and DEVICE_COMMAND.\n");

    /* -----------------------------------------------------------------------
     * C1-2: Encode SENSOR_READING  (25.5 °C = 255, 60.0 %RH = 600)
     * --------------------------------------------------------------------- */
    BuildSensorPayload(255u, 600u, payload);
    ret = MsgCodec_Encode(MSG_TYPE_SENSOR_READING, MSG_PRIORITY_NORMAL, payload, sensorBuf);
    printf("\n[ENCODE] SENSOR_READING  ret=%s\n", (ret == E_OK) ? "E_OK" : "E_NOT_OK");
    printf("  Payload: temp=25.5 degC (255 units)  hum=60.0 %%RH (600 units)\n");

    /* -----------------------------------------------------------------------
     * C1-3: Encode DEVICE_COMMAND  (cmdId=0x01, param=0xFF)
     * --------------------------------------------------------------------- */
    BuildCommandPayload(0x01u, 0xFFu, payload);
    ret = MsgCodec_Encode(MSG_TYPE_DEVICE_COMMAND, MSG_PRIORITY_HIGH, payload, cmdBuf);
    printf("\n[ENCODE] DEVICE_COMMAND  ret=%s\n", (ret == E_OK) ? "E_OK" : "E_NOT_OK");
    printf("  Payload: cmdId=0x01  param=0xFF\n");

    /* -----------------------------------------------------------------------
     * C1-4/5: Decode both messages — handlers print contents
     * --------------------------------------------------------------------- */
    printf("\n--- Decoding messages (handlers will print) ---");
    ret = MsgCodec_Decode((vptr_t)sensorBuf, &decoded);
    (void)ret;

    ret = MsgCodec_Decode((vptr_t)cmdBuf, &decoded);
    (void)ret;

    /* -----------------------------------------------------------------------
     * C2: Memory Scanner Demonstration
     * --------------------------------------------------------------------- */
    printf("\n\n--- C2: Memory Scanner Demonstration ---\n");

    /* C2-6: HexDump both encoded message buffers */
    printf("\n[C2-6] HexDump of sensorBuf : ");
    MemScanner_HexDump((vptr_t)sensorBuf, MSG_SIZE);

    printf("[C2-6] HexDump of cmdBuf    : ");
    MemScanner_HexDump((vptr_t)cmdBuf, MSG_SIZE);

    /* C2-7: ReadByte — HEADER (offset 0) and LENGTH (offset 1) */
    byteVal = MemScanner_ReadByte((vptr_t)sensorBuf, 0u);
    printf("\n[C2-7] sensorBuf HEADER byte (offset 0) : 0x%02X\n", byteVal);
    byteVal = MemScanner_ReadByte((vptr_t)sensorBuf, 1u);
    printf("[C2-7] sensorBuf LENGTH byte (offset 1) : 0x%02X  (%u bytes)\n", byteVal, byteVal);

    /* C2-8: ReadHalfWord — temperature (lower 16 bits of payload at offset 2) */
    halfVal = MemScanner_ReadHalfWord((vptr_t)sensorBuf, 2u);
    printf("\n[C2-8] Temperature (ReadHalfWord offset 2) : 0x%04X -> %.1f degC\n",
           halfVal, (double)halfVal / 10.0);

    /* C2-9: ReadWord — full 32-bit payload at offset 2 */
    wordVal = MemScanner_ReadWord((vptr_t)sensorBuf, 2u);
    printf("\n[C2-9] Full payload (ReadWord offset 2) : 0x%08lX\n", (unsigned long)wordVal);

    /* C2-10: WriteByte — flip PRIORITY bit in a copy of sensorBuf */
    u8 i;
    for (i = 0u; i < MSG_SIZE; i++) { sensorBufCopy[i] = sensorBuf[i]; }

    u8 origHeader  = MemScanner_ReadByte((vptr_t)sensorBufCopy, 0u);
    u8 newHeader   = origHeader;
    TOGGLE_BIT(newHeader, MSG_PRIORITY_BIT);
    MemScanner_WriteByte((vptr_t)sensorBufCopy, 0u, newHeader);

    printf("\n[C2-10] Original sensorBuf  : ");
    MemScanner_HexDump((vptr_t)sensorBuf, MSG_SIZE);
    printf("[C2-10] Modified copy (PRIORITY toggled) : ");
    MemScanner_HexDump((vptr_t)sensorBufCopy, MSG_SIZE);

    /* C2-11: MemFill — fill emptyBuf with 0xAA */
    MemScanner_MemFill((vptr_t)emptyBuf, MSG_SIZE, 0xAAu);
    printf("\n[C2-11] Buffer filled with 0xAA : ");
    MemScanner_HexDump((vptr_t)emptyBuf, MSG_SIZE);

    /* C2-12: MemCompare — encode same sensor message into two buffers */
    BuildSensorPayload(255u, 600u, payload);
    /* Note: seq number will differ here since we already encoded twice; we re-use
       the original sensorBuf and create an identical copy for the comparison. */
    for (i = 0u; i < MSG_SIZE; i++) { sensorBuf2[i] = sensorBuf[i]; }

    cmpResult = MemScanner_MemCompare((vptr_t)sensorBuf, (vptr_t)sensorBuf2, MSG_SIZE);
    printf("\n[C2-12] MemCompare (identical buffers) : %lu  (expected 0)\n",
           (unsigned long)cmpResult);

    /* Introduce a difference at byte 2 and compare again */
    sensorBuf2[2u] ^= 0xFFu;
    cmpResult = MemScanner_MemCompare((vptr_t)sensorBuf, (vptr_t)sensorBuf2, MSG_SIZE);
    printf("[C2-12] MemCompare (byte 2 modified)   : %lu  (expected 3, 1-indexed)\n",
           (unsigned long)cmpResult);

    /* C2-13: FindByte — search for a known payload byte, then a missing byte */
    u8 knownByte = sensorBuf[2u];
    findResult = MemScanner_FindByte((vptr_t)sensorBuf, MSG_SIZE, knownByte);
    printf("\n[C2-13] FindByte(0x%02X) in sensorBuf : offset %d  (found)\n",
           knownByte, findResult);

    findResult = MemScanner_FindByte((vptr_t)sensorBuf, MSG_SIZE, 0xDEu);
    printf("[C2-13] FindByte(0xDE) in sensorBuf : %d  (expected -1, not found)\n",
           findResult);

    printf("\n=== Demonstration complete ===\n");
    return 0;
}
