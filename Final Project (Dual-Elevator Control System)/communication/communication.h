/**
 * @file communication.h
 * @brief SPI IPC packet handling for dual-elevator system.
 */

#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "main/main.h"
#include "exti/exti.h"

#define PACKET_HEADER    0xA5

typedef struct {
    uint8_t header;
    uint8_t elevator_id;
    uint8_t current_floor;
    uint8_t target_floor;
    uint8_t state;
    uint8_t request_bitmap;
    uint8_t status_flags;
    uint8_t checksum;
} ipc_packet_t;

/**
 * @brief Initializes the communication module.
 */
void Communication_Init(void);

/**
 * @brief Processes an incoming packet.
 * @param packet Pointer to received packet
 * @return OK if valid, ERROR otherwise
 */
status_t Communication_Process_Packet(ipc_packet_t *packet);

/**
 * @brief Prepares a packet for transmission.
 * @param packet Pointer to packet to populate
 */
void Communication_Prepare_Packet(ipc_packet_t *packet);

/**
 * @brief Calculates checksum for a packet.
 */
uint8_t Communication_Calc_Checksum(ipc_packet_t *packet);

#endif /* COMMUNICATION_H */
