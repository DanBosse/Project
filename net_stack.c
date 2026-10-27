#include <stdint.h>
#include <stddef.h>

#define MAX_PACKET_SIZE 1514
#define QUEUE_SIZE 64

typedef struct {
    uint8_t data[MAX_PACKET_SIZE];
    uint16_t length;
    uint8_t protocol;
} network_packet_t;

typedef struct {
    network_packet_t queue[QUEUE_SIZE];
    uint32_t head;
    uint32_t tail;
    uint32_t dropped_packets;
} net_ring_buffer_t;

static net_ring_buffer_t rx_buffer;

void net_stack_init(void) {
    rx_buffer.head = 0;
    rx_buffer.tail = 0;
    rx_buffer.dropped_packets = 0;
}

int net_stack_enqueue_packet(const uint8_t *data, uint16_t len, uint8_t proto) {
    if (!data || len > MAX_PACKET_SIZE) return -1;
    uint32_t next_head = (rx_buffer.head + 1) % QUEUE_SIZE;
    if (next_head == rx_buffer.tail) {
        rx_buffer.dropped_packets++;
        return -2; // Buffer full
    }
    
    network_packet_t *pkt = &rx_buffer.queue[rx_buffer.head];
    pkt->length = len;
    pkt->protocol = proto;
    for (uint16_t i = 0; i < len; i++) {
        pkt->data[i] = data[i];
    }
    
    rx_buffer.head = next_head;
    return 0;
}

int net_send_ping(const char *dest) {
    if (!dest) return -1;
    // Transmit ICMP echo request stub
    return 0;
}

void net_print_status(void) {
    // Output network telemetry status to console/serial
}
