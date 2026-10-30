/*
 * net_stack.c - TorusOS Network Stack with NP-Complete Handshake Intercept
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "net_stack.h"

// External functions
extern void kernel_print(const char* str);
extern void np_handshake_init(void);
extern int np_handshake_issue_challenge(uint32_t client_ip, uint32_t *set_out, uint32_t *target_out);
extern int np_handshake_verify_solution(uint32_t client_ip, uint16_t client_mask);

void net_init(void) {
    np_handshake_init();
    kernel_print("[NET_STACK] Network Stack initialized with NP-Complete Handshake defense.\n");
}

int net_stack_handle_packet(uint32_t src_ip, uint32_t dst_ip, uint16_t port, const uint8_t* payload, size_t len) {
    (void)dst_ip;
    (void)port;
    // Intercept incoming packets for NP-Complete Handshake evaluation (SYN flag simulated via payload byte 0x02)
    if (len >= 1 && payload[0] == 0x02) { 
        uint32_t set_vals[16];
        uint32_t target = 0;
        if (np_handshake_issue_challenge(src_ip, set_vals, &target) < 0) {
            kernel_print("[NET_STACK] [DROP] Connection flood detected. Challenge queue saturated.\n");
            return -1;
        }
        return 1; // Challenge issued
    }
    return 0;
}
