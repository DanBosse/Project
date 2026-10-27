#include <stdint.h>
#include <stddef.h>

#define MAX_WATCHDOG_TIERS 6
#define HEARTBEAT_TIMEOUT_TICKS 1000

typedef struct {
    uint32_t tier_id;
    uint64_t last_heartbeat;
    uint32_t failure_count;
    uint8_t healthy;
} tier_health_record_t;

static tier_health_record_t watchdog_registry[MAX_WATCHDOG_TIERS];

void watchdog_healing_init(void) {
    for (int i = 0; i < MAX_WATCHDOG_TIERS; i++) {
        watchdog_registry[i].tier_id = i + 1;
        watchdog_registry[i].last_heartbeat = 0;
        watchdog_registry[i].failure_count = 0;
        watchdog_registry[i].healthy = 1;
    }
}

int watchdog_heartbeat(uint32_t tier_id, uint64_t current_tick) {
    if (tier_id == 0 || tier_id > MAX_WATCHDOG_TIERS) return -1;
    
    tier_health_record_t *rec = &watchdog_registry[tier_id - 1];
    rec->last_heartbeat = current_tick;
    rec->healthy = 1;
    return 0;
}

int watchdog_check_health(uint64_t current_tick) {
    int failures_detected = 0;
    for (int i = 0; i < MAX_WATCHDOG_TIERS; i++) {
        tier_health_record_t *rec = &watchdog_registry[i];
        if (current_tick > rec->last_heartbeat + HEARTBEAT_TIMEOUT_TICKS) {
            if (rec->healthy) {
                rec->healthy = 0;
                rec->failure_count++;
            }
            failures_detected++;
        }
    }
    return failures_detected;
}
