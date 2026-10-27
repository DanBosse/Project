#include "ipc.h"
#include "ring_handshake.h"

void ipc_init(void) {
    for (int i = 0; i < MAX_TIERS; i++) {
        ring_handshake_init((uint8_t)i);
    }
}

int ipc_send(uint8_t tier_id, const ipc_message_t *msg) {
    if (tier_id >= MAX_TIERS || !msg) return -1;
    return ring_handshake_send(tier_id, msg);
}

int ipc_receive(uint8_t tier_id, ipc_message_t *msg) {
    if (tier_id >= MAX_TIERS || !msg) return -1;
    return ring_handshake_receive(tier_id, msg);
}
