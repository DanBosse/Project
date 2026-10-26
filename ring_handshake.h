#ifndef RING_HANDSHAKE_H
#define RING_HANDSHAKE_H

#include <stdint.h>
#include <stdbool.h>

#define CAP_ALLOW_IPC         (1 << 0)
#define CAP_ALLOW_TELEMETRY   (1 << 1)
#define CAP_ALLOW_SPATIAL_UI  (1 << 2)

typedef struct {
    uint32_t container_id;
    uint64_t cryptographic_nonce;
    bool attestation_valid;
    uint32_t granted_capabilities;
} RingHandshakeContext;

RingHandshakeContext kernel_perform_ring_handshake(uint32_t container_id);

#endif
