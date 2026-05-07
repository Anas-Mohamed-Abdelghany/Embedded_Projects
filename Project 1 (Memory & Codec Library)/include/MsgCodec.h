#ifndef MSGCODEC_H
#define MSGCODEC_H

#include "STD_TYPES.h"

/* -------------------------------------------------------------------------
 * Message layout constants
 * ------------------------------------------------------------------------- */
#define MSG_SIZE              (6u)   /* Total message size in bytes (always fixed) */
#define MSG_PAYLOAD_SIZE      (4u)   /* Payload field size in bytes               */

/* -------------------------------------------------------------------------
 * HEADER byte bit-field positions and widths
 * ------------------------------------------------------------------------- */
#define MSG_TYPE_START        (0u)
#define MSG_TYPE_WIDTH        (3u)
#define MSG_PRIORITY_BIT      (3u)
#define MSG_SEQ_NUM_START     (4u)
#define MSG_SEQ_NUM_WIDTH     (4u)

/* -------------------------------------------------------------------------
 * Message types
 * ------------------------------------------------------------------------- */
#define MSG_TYPE_SENSOR_READING  (0u)
#define MSG_TYPE_DEVICE_COMMAND  (1u)
#define MSG_TYPE_COUNT           (8u)   /* 3-bit field supports 0–7 */

/* -------------------------------------------------------------------------
 * Priority values
 * ------------------------------------------------------------------------- */
#define MSG_PRIORITY_NORMAL   (0u)
#define MSG_PRIORITY_HIGH     (1u)

/* -------------------------------------------------------------------------
 * SensorPayload — allows access as 32-bit, two 16-bit halves, or four bytes.
 * Lower 16 bits = temperature (0.1 °C units).
 * Upper 16 bits = humidity   (0.1 %RH units).
 * ------------------------------------------------------------------------- */
typedef union
{
    u32 raw32;                /* Full 32-bit value           */
    u16 half[2u];             /* [0]=temperature, [1]=humidity */
    u8  bytes[4u];            /* Individual bytes            */
} SensorPayload_t;

/* -------------------------------------------------------------------------
 * DeviceCommandPayload
 * ------------------------------------------------------------------------- */
typedef struct
{
    u8 cmdId;
    u8 param;
    u8 reserved[2u];
} DeviceCommandPayload_t;

/* -------------------------------------------------------------------------
 * Message struct — maps to the 6-byte wire layout.
 * Size is verified in MsgCodec.c via a compile-time assertion.
 * ------------------------------------------------------------------------- */
typedef struct
{
    u8 header;                /* Byte 0: MSG_TYPE[2:0] | PRIORITY[3] | SEQ_NUM[7:4] */
    u8 length;                /* Byte 1: payload size in bytes (always 4)            */
    u8 payload[MSG_PAYLOAD_SIZE]; /* Bytes 2–5: raw payload bytes                   */
} Message_t;

/* -------------------------------------------------------------------------
 * Decoded message result handed to handlers / returned by decoder.
 * ------------------------------------------------------------------------- */
typedef struct
{
    u8      msgType;
    u8      priority;
    u8      seqNum;
    u8      length;

    /* Type-specific payload views */
    SensorPayload_t        sensor;   /* valid when msgType == MSG_TYPE_SENSOR_READING */
    DeviceCommandPayload_t command;  /* valid when msgType == MSG_TYPE_DEVICE_COMMAND  */
} DecodedMessage_t;

/* -------------------------------------------------------------------------
 * Handler function pointer type.
 * Registered handlers are called automatically after a successful decode.
 * ------------------------------------------------------------------------- */
typedef void (*MsgHandler_t)(const DecodedMessage_t *pMsg);

/* -------------------------------------------------------------------------
 * Public API
 * ------------------------------------------------------------------------- */

/*
 * Register a handler for a given message type.
 */
Std_ReturnType MsgCodec_RegisterHandler(u8 msgType, MsgHandler_t handler);

/*
 * Encode a message into a 6-byte output buffer.
 */
Std_ReturnType MsgCodec_Encode(u8 msgType, u8 priority,
                               const u8 *pPayload, u8 *pOutBuf);

/*
 * Decode a raw 6-byte buffer into a DecodedMessage_t.
 */
Std_ReturnType MsgCodec_Decode(vptr_t pRawBuf, DecodedMessage_t *pResult);

#endif /* MSGCODEC_H */
