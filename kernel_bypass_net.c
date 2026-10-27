#include <stdint.h>
#include <stddef.h>

#define MAX_RING_SLOTS 64
#define PKT_BUFFER_SIZE 1536

typedef struct {
    uint32_t packet_id;
    uint32_t source_node;
    uint32_t dest_node;
    uint8_t payload[PKT_BUFFER_SIZE];
    size_t payload_len;
    uint8_t processed;
} net_packet_desc_t;

typedef struct {
    uint32_t ring_id;
    net_packet_desc_t slots[MAX_RING_SLOTS];
    uint32_t head;
    uint32_t tail;
    uint32_t active_count;
    uint8_t online;
} zero_copy_ring_t;

static zero_copy_ring_t network_rings[4]; // TX/RX rings
static uint32_t next_packet_id = 10000;

void kernel_bypass_net_init(void) {
    for (int i = 0; i < 4; i++) {
        network_rings[i].ring_id = 900 + i;
        network_rings[i].head = 0;
        network_rings[i].tail = 0;
        network_rings[i].active_count = 0;
        network_rings[i].online = 1;
    }
}

int kernel_bypass_transmit(uint32_t dest_node, const uint8_t *data, size_t size, uint32_t *out_packet_id) {
    if (!data || size == 0 || size > PKT_BUFFER_SIZE || !out_packet_id) return -1;

    zero_copy_ring_t *tx_ring = &network_rings[0]; // Ring 0 designated for TX

    if (tx_ring->active_count >= MAX_RING_SLOTS) {
        return -2; // TX ring buffer full (backpressure event)
    }

    uint32_t tail_idx = tx_ring->tail;
    net_packet_desc_t *pkt = &tx_ring->slots[tail_idx];

    pkt->packet_id = next_packet_id++;
    pkt->source_node = 0x01; // Local node ID stub
    pkt->dest_node = dest_node;
    pkt->payload_len = size;
    pkt->processed = 0;

    for (size_t i = 0; i < size; i++) {
        pkt->payload[i] = data[i];
    }

    tx_ring->tail = (tail_idx + 1) % MAX_RING_SLOTS;
    tx_ring->active_count++;

    *out_packet_id = pkt->packet_id;
    return 0;
}

int kernel_bypass_poll_rx(uint8_t *out_buffer, size_t *out_size, uint32_t *out_source_node) {
    if (!out_buffer || !out_size || !out_source_node) return -1;

    zero_copy_ring_t *rx_ring = &network_rings[1]; // Ring 1 designated for RX

    if (rx_ring->active_count == 0) {
        return -3; // No incoming packets in RX ring
    }

    uint32_t head_idx = rx_ring->head;
    net_packet_desc_t *pkt = &rx_ring->slots[head_idx];

    if (pkt->payload_len > *out_size) {
        return -4; // Destination buffer too small
    }

    for (size_t i = 0; i < pkt->payload_len; i++) {
        out_buffer[i] = pkt->payload[i];
    }

    *out_size = pkt->payload_len;
    *out_source_node = pkt->source_node;

    rx_ring->head = (head_idx + 1) % MAX_RING_SLOTS;
    rx_ring->active_count--;

    return 0;
}
