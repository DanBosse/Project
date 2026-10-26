#ifndef VIRTIO_NET_H
#define VIRTIO_NET_H

#include <stdint.h>
#include <stdbool.h>

// Virtio Net Header structure prepended to packets
typedef struct {
    uint8_t flags;
    uint8_t gso_type;
    uint16_t hdr_len;
    uint16_t gso_size;
    uint16_t csum_start;
    uint16_t csum_offset;
    uint16_t num_buffers;
} __attribute__((packed)) virtio_net_hdr_t;

// Initialize Virtio-Net PCI interface
void virtio_net_init(void);

// Transmit a raw Ethernet frame
bool virtio_net_send(const uint8_t* packet, uint32_t length);

// Poll for incoming packets
bool virtio_net_poll(uint8_t* buffer, uint32_t* length);

#endif
