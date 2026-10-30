#include <stdint.h>

extern uint32_t firewall_get_rolling_secret(uint32_t uptime_sec);
extern int firewall_is_ip_blacklisted(uint32_t client_ip, uint32_t uptime_sec);

int np_handshake_verify_solution_hardened(uint32_t client_ip, uint32_t nonce, uint32_t client_solution, uint32_t uptime_sec) {
    // Drop instantly if IP is in Ring 0 blacklist
    if (firewall_is_ip_blacklisted(client_ip, uptime_sec)) {
        return -1;
    }

    // Compute expected answer using rolling secret
    uint32_t secret = firewall_get_rolling_secret(uptime_sec);
    uint32_t h = client_ip ^ secret;
    h ^= nonce;
    h ^= h >> 16; h *= 0x85ebca6b;
    h ^= h >> 13; h *= 0xc2b2ae35;
    h ^= h >> 16;

    if (h == client_solution) {
        return 0; // Authenticated
    }

    return -2; // Spoofed / Verification failed
}
