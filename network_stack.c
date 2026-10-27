#include <stdint.h>
#include <stddef.h>

#define MAX_PACKET_SLOTS 64
#define PKT_BUFFER_SIZE 2048

typedef struct {
    uint32_t packet_id;
    uint32_t source_node_id;
    uint32_t dest_node_id;
    uint8_t payload[PKT_BUFFER_SIZE];
    size_t length;
    uint8_t active;
} net_packet_slot_t;

typedef struct {
    net_packet_slot_t tx_ring[MAX_PACKET_SLOTS];
    net_packet_slot_t rx_ring[MAX_PACKET_SLOTS];
    uint32_t tx_head;
    uint32_t tx_tail;
    uint32_t rx_head;
    uint32_t rx_tail;
} kernel_bypass_net_t;

static kernel_bypass_net_t net_stack;
static uint32_t next_packet_id = 50000;

void network_stack_init(void) {
    net_stack.tx_head = 0;
    net_stack.tx_tail = 0;
    net_stack.rx_head = 0;
    net_stack.rx_tail = 0;
    
    for (int i = 0; i < MAX_PACKET_SLOTS; i++) {
        net_stack.tx_ring[i].active = 0;
        net_stack.rx_ring[i].active = 0;
    }
}

int network_stack_send_zero_copy(uint32_t dest_node, const uint8_t *data, size_t size, uint32_t *out_pkt_id) {
    if (!data || size == 0 || size > PKT_BUFFER_SIZE || !out_pkt_id) return -1;

    uint32_t next_head = (net_stack.tx_head + 1) % MAX_PACKET_SLOTS;
    if (next_head == net_stack.tx_tail) {
        return -2; // TX ring buffer full (backpressure triggered)
    }

    net_packet_slot_t *pkt = &net_stack.tx_ring[net_stack.tx_head];
    pkt->packet_id = next_packet_id++;
    pkt->dest_node_id = dest_node;
    pkt->source_node_id = 0x7A4F8821; // Local sovereign node ID stub
    pkt->length = size;

    for (size_t i = 0; i < size; i++) {
        pkt->payload[i] = data[i];
    }
    pkt->active = 1;

    net_stack.tx_head = next_head;
    *out_pkt_id = pkt->packet_id;
    return 0;
}

int network_stack_poll_rx(uint8_t *out_buffer, size_t *out_size, uint32_t *src_node_out) {
    if (net_stack.rx_head == net_stack.rx_tail) {
        return -3; // No incoming packets in RX ring
    }

    net_packet_slot_t *pkt = &net_stack.rx_ring[net_stack.rx_tail];
    if (!pkt->active) {
        return -4;
    }

    if (out_buffer && out_size) {
        size_t copy_len = (pkt->length < *out_size) ? pkt->length : *out_size;
        for (size_t i = 0; i < copy_len; i++) {
            out_buffer[i] = pkt->payload[i];
        }
        *out_size = copy_len;
    }

    if (src_node_out) {
        *src_node_out = pkt->source_node_id;
    }

    pkt->active = 0;
    net_stack.rx_tail = (net_stack.rx_tail + 1) % MAX_PACKET_SLOTS;
    return 0;
}
