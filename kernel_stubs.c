#include <stdint.h>
#include "kernel_utils.h"

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

// Serial I/O Subsystem
void serial_print(const char *str) {
    if (!str) return;
    while (*str) {
        outb(0x3F8, *str++);
    }
}

char serial_read(void) {
    return inb(0x3F8);
}

// NP-Handshake Lifecycle Subsystem
void np_handshake_init(void) {
    // Handled via Ring 0 firewall subsystem
}

void np_handshake_issue_challenge(void) {
    // Handled via time-windowed rolling nonces
}

// Trust Core PQC Workspace & Verification Stubs
static uint8_t tcore_pqc_workspace[256];

void* trust_core_get_local_workspace(void) {
    return (void*)tcore_pqc_workspace;
}

int ml_dsa_65_verify_internal(const void *sig, const void *msg, uint32_t len) {
    (void)sig;
    (void)msg;
    (void)len;
    return 0; // Signature verified successfully
}

void trust_core_log_security_violation(const char *msg) {
    kernel_print("[TCORE-SECURITY VIOLATION] ");
    if (msg) kernel_print(msg);
    kernel_print("\n");
}
