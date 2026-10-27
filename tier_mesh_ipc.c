#include <stdint.h>
#include <stddef.h>

#define MAX_MESH_CHANNELS 8
#define MESH_RING_SIZE 256

typedef struct {
    uint32_t sender_tier;
    uint32_t receiver_tier;
    uint32_t message_id;
    uint64_t payload;
} mesh_message_t;

typedef struct {
    mesh_message_t buffer[MESH_RING_SIZE];
    volatile uint32_t head;
    volatile uint32_t tail;
    uint8_t active;
} mesh_channel_t;

static mesh_channel_t tier_mesh[MAX_MESH_CHANNELS];

void tier_mesh_init(void) {
    for (int i = 0; i < MAX_MESH_CHANNELS; i++) {
        tier_mesh[i].head = 0;
        tier_mesh[i].tail = 0;
        tier_mesh[i].active = 1;
    }
}

int tier_mesh_broadcast(uint32_t channel_id, uint32_t sender, uint32_t msg_id, uint64_t payload) {
    if (channel_id >= MAX_MESH_CHANNELS || !tier_mesh[channel_id].active) {
        return -1;
    }
    
    mesh_channel_t *ch = &tier_mesh[channel_id];
    uint32_t next_head = (ch->head + 1) % MESH_RING_SIZE;
    
    if (next_head == ch->tail) {
        return -2; // Mesh channel ring full
    }
    
    ch->buffer[ch->head].sender_tier = sender;
    ch->buffer[ch->head].message_id = msg_id;
    ch->buffer[ch->head].payload = payload;
    ch->head = next_head;
    
    return 0;
}

int tier_mesh_poll(uint32_t channel_id, mesh_message_t *out_msg) {
    if (channel_id >= MAX_MESH_CHANNELS || !out_msg || !tier_mesh[channel_id].active) {
        return -1;
    }
    
    mesh_channel_t *ch = &tier_mesh[channel_id];
    if (ch->head == ch->tail) {
        return -3; // No messages pending
    }
    
    *out_msg = ch->buffer[ch->tail];
    ch->tail = (ch->tail + 1) % MESH_RING_SIZE;
    
    return 0;
}
