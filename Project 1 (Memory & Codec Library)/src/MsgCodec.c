#include <string.h>
#include "MsgCodec.h"
#include "BIT_MATH.h"

/* -------------------------------------------------------------------------
 * Compile-time size assertion — Message_t must be exactly 6 bytes.
 * ------------------------------------------------------------------------- */
typedef u8 _assert_msg_size[(sizeof(Message_t) == 6u) ? 1 : -1];

/* -------------------------------------------------------------------------
 * Module-private state (not visible outside this file)
 * ------------------------------------------------------------------------- */
static MsgHandler_t s_handlers[MSG_TYPE_COUNT];
static u8           s_seqNum = 0u;

/* -------------------------------------------------------------------------
 * MsgCodec_RegisterHandler
 * ------------------------------------------------------------------------- */
Std_ReturnType MsgCodec_RegisterHandler(u8 msgType, MsgHandler_t handler)
{
    if (msgType >= MSG_TYPE_COUNT)
    {
        return E_NOT_OK;
    }
    s_handlers[msgType] = handler;
    return E_OK;
}

/* -------------------------------------------------------------------------
 * MsgCodec_Encode
 * ------------------------------------------------------------------------- */
Std_ReturnType MsgCodec_Encode(u8 msgType, u8 priority,
                               const u8 *pPayload, u8 *pOutBuf)
{
    u8 header = 0u;

    if ((pPayload == NULL) || (pOutBuf == NULL) || (msgType >= MSG_TYPE_COUNT))
    {
        return E_NOT_OK;
    }

    /* Build HEADER byte using BIT_MATH macros */
    WRITE_FIELD(header, MSG_TYPE_START,    MSG_TYPE_WIDTH,   msgType);
    WRITE_FIELD(header, MSG_PRIORITY_BIT,  1u,               priority);
    WRITE_FIELD(header, MSG_SEQ_NUM_START, MSG_SEQ_NUM_WIDTH, s_seqNum & 0x0Fu);

    pOutBuf[0u] = header;
    pOutBuf[1u] = (u8)MSG_PAYLOAD_SIZE;

    /* Copy payload bytes */
    pOutBuf[2u] = pPayload[0u];
    pOutBuf[3u] = pPayload[1u];
    pOutBuf[4u] = pPayload[2u];
    pOutBuf[5u] = pPayload[3u];

    /* Auto-increment sequence number (wraps at 4-bit boundary) */
    s_seqNum = (u8)((s_seqNum + 1u) & 0x0Fu);

    return E_OK;
}

/* -------------------------------------------------------------------------
 * MsgCodec_Decode
 * ------------------------------------------------------------------------- */
Std_ReturnType MsgCodec_Decode(vptr_t pRawBuf, DecodedMessage_t *pResult)
{
    const u8 *pBuf = (const u8 *)pRawBuf;
    u8        header;
    u8        msgType;

    if ((pBuf == NULL) || (pResult == NULL))
    {
        return E_NOT_OK;
    }

    header  = pBuf[0u];
    msgType = (u8)READ_FIELD(header, MSG_TYPE_START, MSG_TYPE_WIDTH);

    if (msgType >= MSG_TYPE_COUNT)
    {
        return E_NOT_OK;
    }

    /* Extract header fields */
    pResult->msgType  = msgType;
    pResult->priority = (u8)READ_BIT(header, MSG_PRIORITY_BIT);
    pResult->seqNum   = (u8)READ_FIELD(header, MSG_SEQ_NUM_START, MSG_SEQ_NUM_WIDTH);
    pResult->length   = pBuf[1u];

    /* Populate type-specific payload views */
    if (msgType == MSG_TYPE_SENSOR_READING)
    {
        /* Little-Endian reconstruction into the union */
        pResult->sensor.bytes[0u] = pBuf[2u];
        pResult->sensor.bytes[1u] = pBuf[3u];
        pResult->sensor.bytes[2u] = pBuf[4u];
        pResult->sensor.bytes[3u] = pBuf[5u];
    }
    else if (msgType == MSG_TYPE_DEVICE_COMMAND)
    {
        pResult->command.cmdId       = pBuf[2u];
        pResult->command.param       = pBuf[3u];
        pResult->command.reserved[0u] = pBuf[4u];
        pResult->command.reserved[1u] = pBuf[5u];
    }
    else
    {
        /* Unknown type — no handler will be called */
    }

    /* Dispatch to registered handler if one exists */
    if (s_handlers[msgType] != NULL)
    {
        s_handlers[msgType](pResult);
    }

    return E_OK;
}
