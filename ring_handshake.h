#ifndef RING_HANDSHAKE_H
#define RING_HANDSHAKE_H

#include <stdint.h>
#include "ipc.h"

typedef struct {
    uint8_t tier_id;
    uint32_t status;
    int attestation_valid;
} RingHandshakeContext;

int ring_handshake_init(uint8_t tier_id);
int ring_handshake_send(uint8_t tier_id, const ipc_message_t *msg);
int ring_handshake_receive(uint8_t tier_id, ipc_message_t *msg);
RingHandshakeContext kernel_perform_ring_handshake(uint8_t tier_id);

#endif
