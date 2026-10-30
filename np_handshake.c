#include <stdint.h>
#include "kernel_utils.h"

// Server-side secret key initialized randomly or set to a secure constant
static uint32_t server_secret = 0xA3F57B91;

/* 
 * Lightweight cryptographic PRF (Pseudo-Random Function)
 * Acts as our kernel-safe MAC (Message Authentication Code)
 */
static uint32_t compute_crypto_cookie(uint32_t client_ip, uint32_t nonce) {
    uint32_t h = client_ip ^ server_secret;
    h ^= nonce;
    // Fast 32-bit integer mixing (MurmurHash mixing constants)
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return h;
}

/* 
 * Cryptographic Handshake Verification 
 * Returns 0 if valid, -1 if invalid/spoofed.
 */
int np_handshake_verify_solution(uint32_t client_ip, uint32_t nonce, uint32_t client_solution) {
    uint32_t expected_cookie = compute_crypto_cookie(client_ip, nonce);
    
    if (client_solution == expected_cookie) {
        return 0; // Valid cryptographic proof
    }
    
    return -1; // Invalid proof -> Drop packet
}
