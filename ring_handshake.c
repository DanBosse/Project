#include "ring_handshake.h"

#define RING_BUFFER_SIZE 256

typedef struct {
    uint32_t head;
    uint32_t tail;
    uint32_t magic_sync;
    ipc_message_t messages[RING_BUFFER_SIZE];
} secure_ring_t;

static secure_ring_t kernel_ring_channels[MAX_TIERS];

int ring_handshake_init(uint8_t tier_id) {
    if (tier_id >= MAX_TIERS) return -1;
    secure_ring_t *ring = &kernel_ring_channels[tier_id];
    ring->head = 0;
    ring->tail = 0;
    ring->magic_sync = 0x5EC45EC4; // Valid hex literal sync magic
    return 0;
}

int ring_handshake_send(uint8_t tier_id, const ipc_message_t *msg) {
    if (tier_id >= MAX_TIERS || !msg) return -1;
    secure_ring_t *ring = &kernel_ring_channels[tier_id];
    uint32_t next_head = (ring->head + 1) % RING_BUFFER_SIZE;

    if (next_head == ring->tail) {
        return -1; 
    }

    ring->messages[ring->head] = *msg;
    __asm__ volatile("" ::: "memory");
    ring->head = next_head;
    return 0;
}

int ring_handshake_receive(uint8_t tier_id, ipc_message_t *msg) {
    if (tier_id >= MAX_TIERS || !msg) return -1;
    secure_ring_t *ring = &kernel_ring_channels[tier_id];

    if (ring->head == ring->tail) {
        return 0; 
    }

    *msg = ring->messages[ring->tail];
    __asm__ volatile("" ::: "memory");
    ring->tail = (ring->tail + 1) % RING_BUFFER_SIZE;
    return 1;
}

RingHandshakeContext kernel_perform_ring_handshake(uint8_t tier_id) {
    RingHandshakeContext ctx;
    ctx.tier_id = tier_id;
    ctx.status = 0;
    
    int res = ring_handshake_init(tier_id);
    ctx.attestation_valid = (res == 0) ? 1 : 0;
    return ctx;
}
