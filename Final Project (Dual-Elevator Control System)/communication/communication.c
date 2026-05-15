/**
 * @file communication.c
 * @brief Communication module implementation.
 */

#include "communication.h"
#include "communication_private.h"

void Communication_Init(void) {
    /* Initialize communication state */
}

uint8_t Communication_Calc_Checksum(ipc_packet_t *packet) {
    uint8_t sum = 0;
    uint8_t *ptr = (uint8_t *)packet;
    
    for (uint8_t i = 0; i < 7; i++) {
        sum += ptr[i];
    }
    return sum;
}

status_t Communication_Process_Packet(ipc_packet_t *packet) {
    /* 1. Verify Header */
    if (packet->header != PACKET_HEADER) {
        return ERROR;
    }
    
    /* 2. Verify Checksum */
    uint8_t calc_sum = Communication_Calc_Checksum(packet);
    if (calc_sum != packet->checksum) {
        return ERROR;
    }
    
    return OK;
}

void Communication_Prepare_Packet(ipc_packet_t *packet) {
    packet->header = PACKET_HEADER;
    packet->checksum = Communication_Calc_Checksum(packet);
}
