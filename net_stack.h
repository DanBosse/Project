#ifndef NET_STACK_H
#define NET_STACK_H

#include <stdint.h>
#include <stdbool.h>

// Initialize the Sovereign Network Stack
void net_stack_init(void);

// Process incoming raw Ethernet frames from Virtio-Net
void net_receive_packet(const uint8_t* frame, uint32_t length);

// Transmit an ICMP Echo Request (Ping)
void net_send_ping(uint32_t target_ip);

// Retrieve network statistics for diagnostics
void net_print_status(void);

#endif
