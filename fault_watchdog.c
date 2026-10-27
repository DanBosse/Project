#include <stdint.h>
#include <stddef.h>

#define MAX_WATCHED_ENTITIES 32

typedef enum {
    ENTITY_TYPE_MICROVM = 1,
    ENTITY_TYPE_KERNEL_THREAD,
    ENTITY_TYPE_PEER_NODE,
    ENTITY_TYPE_ACCELERATOR
} watched_entity_type_t;

typedef struct {
    uint32_t entity_id;
    watched_entity_type_t type;
    uint64_t last_heartbeat_tick;
    uint64_t timeout_threshold_ticks;
    uint32_t failure_count;
    uint32_t max_retries;
    uint8_t active;
} watchdog_entity_t;

static watchdog_entity_t watchdog_registry[MAX_WATCHED_ENTITIES];

void fault_watchdog_init(void) {
    for (int i = 0; i < MAX_WATCHED_ENTITIES; i++) {
        watchdog_registry[i].active = 0;
        watchdog_registry[i].failure_count = 0;
    }
}

int fault_watchdog_register(uint32_t entity_id, watched_entity_type_t type, uint64_t timeout_ticks, uint32_t max_retries) {
    for (int i = 0; i < MAX_WATCHED_ENTITIES; i++) {
        if (!watchdog_registry[i].active) {
            watchdog_registry[i].entity_id = entity_id;
            watchdog_registry[i].type = type;
            watchdog_registry[i].timeout_threshold_ticks = timeout_ticks;
            watchdog_registry[i].max_retries = max_retries;
            watchdog_registry[i].failure_count = 0;
            watchdog_registry[i].last_heartbeat_tick = 0;
            watchdog_registry[i].active = 1;
            return 0;
        }
    }
    return -1; // Watchdog registry full
}

int fault_watchdog_heartbeat(uint32_t entity_id, uint64_t current_tick) {
    for (int i = 0; i < MAX_WATCHED_ENTITIES; i++) {
        if (watchdog_registry[i].active && watchdog_registry[i].entity_id == entity_id) {
            watchdog_registry[i].last_heartbeat_tick = current_tick;
            watchdog_registry[i].failure_count = 0; // Reset failure count on valid pulse
            return 0;
        }
    }
    return -2; // Entity ID not found in watchdog registry
}

int fault_watchdog_evaluate_health(uint64_t current_tick, uint32_t *out_failed_entity_id) {
    if (!out_failed_entity_id) return -1;

    for (int i = 0; i < MAX_WATCHED_ENTITIES; i++) {
        if (watchdog_registry[i].active) {
            uint64_t elapsed = current_tick - watchdog_registry[i].last_heartbeat_tick;
            if (elapsed > watchdog_registry[i].timeout_threshold_ticks) {
                watchdog_registry[i].failure_count++;
                *out_failed_entity_id = watchdog_registry[i].entity_id;

                if (watchdog_registry[i].failure_count > watchdog_registry[i].max_retries) {
                    watchdog_registry[i].active = 0; // Terminate/evict dead entity
                    return -3; // Unrecoverable entity failure (trigger failover/restart)
                }
                return -2; // Entity missed heartbeat window (warning / recovery initiated)
            }
        }
    }
    return 0; // All monitored entities healthy
}
