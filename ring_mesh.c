#include <stdint.h>
#include <stddef.h>

#define MAX_TIERS 6
#define RING_BUFFER_CAPACITY 256
#define MSG_PAYLOAD_SIZE 64

typedef struct {
    uint32_t sender_tier;
    uint32_t receiver_tier;
    uint32_t msg_type;
    uint8_t payload[MSG_PAYLOAD_SIZE];
    uint32_t sequence;
} ring_message_t;

typedef struct {
    ring_message_t buffer[RING_BUFFER_CAPACITY];
    volatile uint32_t head;
    volatile uint32_t tail;
} tier_ring_channel_t;

static tier_ring_channel_t inter_tier_mesh[MAX_TIERS][MAX_TIERS];

void ring_mesh_init(void) {
    for (int i = 0; i < MAX_TIERS; i++) {
        for (int j = 0; j < MAX_TIERS; j++) {
            inter_tier_mesh[i][j].head = 0;
            inter_tier_mesh[i][j].tail = 0;
        }
    }
}

int ring_mesh_send(uint32_t src_tier, uint32_t dest_tier, uint32_t msg_type, const uint8_t *data, size_t size) {
    if (src_tier == 0 || src_tier > MAX_TIERS || dest_tier == 0 || dest_tier > MAX_TIERS) return -1;
    if (size > MSG_PAYLOAD_SIZE) return -2;

    tier_ring_channel_t *channel = &inter_tier_mesh[src_tier - 1][dest_tier - 1];
    uint32_t head = channel->head;
    uint32_t next_head = (head + 1) % RING_BUFFER_CAPACITY;

    if (next_head == channel->tail) {
        return -3; // Ring buffer full (backpressure triggered)
    }

    ring_message_t *msg = &channel->buffer[head];
    msg->sender_tier = src_tier;
    msg->receiver_tier = dest_tier;
    msg->msg_type = msg_type;
    msg->sequence = head;

    for (size_t i = 0; i < size; i++) {
        msg->payload[i] = data[i];
    }
    for (size_t i = size; i < MSG_PAYLOAD_SIZE; i++) {
        msg->payload[i] = 0;
    }

    channel->head = next_head;
    return 0;
}

int ring_mesh_receive(uint32_t src_tier, uint32_t dest_tier, ring_message_t *out_msg) {
    if (src_tier == 0 || src_tier > MAX_TIERS || dest_tier == 0 || dest_tier > MAX_TIERS || !out_msg) return -1;

    tier_ring_channel_t *channel = &inter_tier_mesh[src_tier - 1][dest_tier - 1];
    uint32_t tail = channel->tail;

    if (channel->head == tail) {
        return -2; // No messages in ring buffer channel
    }

    ring_message_t *msg = &channel->buffer[tail];
    out_msg->sender_tier = msg->sender_tier;
    out_msg->receiver_tier = msg->receiver_tier;
    out_msg->msg_type = msg->msg_type;
    out_msg->sequence = msg->sequence;

    for (size_t i = 0; i < MSG_PAYLOAD_SIZE; i++) {
        out_msg->payload[i] = msg->payload[i];
    }

    channel->tail = (tail + 1) % RING_BUFFER_CAPACITY;
    return 0;
}
