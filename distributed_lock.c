#include <stdint.h>
#include <stddef.h>

#define MAX_DISTRIBUTED_LOCKS 32
#define LOCK_NAME_LEN 32

typedef struct {
    uint32_t lock_id;
    char resource_name[LOCK_NAME_LEN];
    uint32_t owner_node_id;
    uint8_t locked;
    uint64_t lease_expiration_tick;
} distributed_lock_entry_t;

static distributed_lock_entry_t lock_manager_table[MAX_DISTRIBUTED_LOCKS];
static uint32_t next_lock_id = 8000;

void distributed_lock_init(void) {
    for (int i = 0; i < MAX_DISTRIBUTED_LOCKS; i++) {
        lock_manager_table[i].locked = 0;
        lock_manager_table[i].owner_node_id = 0;
    }
}

int distributed_lock_acquire(const char *resource_name, uint32_t node_id, uint64_t lease_ticks, uint32_t *out_lock_id) {
    if (!resource_name || !out_lock_id) return -1;

    // Check if lock already exists and is active
    for (int i = 0; i < MAX_DISTRIBUTED_LOCKS; i++) {
        if (lock_manager_table[i].locked) {
            int match = 1;
            for (int j = 0; j < LOCK_NAME_LEN; j++) {
                if (lock_manager_table[i].resource_name[j] != resource_name[j]) {
                    match = 0;
                    break;
                }
                if (resource_name[j] == '\0') break;
            }
            if (match) {
                if (lock_manager_table[i].owner_node_id == node_id) {
                    *out_lock_id = lock_manager_table[i].lock_id;
                    return 0; // Already held by requester
                }
                return -2; // Lock currently held by another node
            }
        }
    }

    // Find free slot
    for (int i = 0; i < MAX_DISTRIBUTED_LOCKS; i++) {
        if (!lock_manager_table[i].locked) {
            lock_manager_table[i].lock_id = next_lock_id++;
            int j = 0;
            while (resource_name[j] != '\0' && j < LOCK_NAME_LEN - 1) {
                lock_manager_table[i].resource_name[j] = resource_name[j];
                j++;
            }
            lock_manager_table[i].resource_name[j] = '\0';
            lock_manager_table[i].owner_node_id = node_id;
            lock_manager_table[i].locked = 1;
            lock_manager_table[i].lease_expiration_tick = lease_ticks;

            *out_lock_id = lock_manager_table[i].lock_id;
            return 0;
        }
    }
    return -3; // Lock table full
}

int distributed_lock_release(uint32_t lock_id, uint32_t node_id) {
    for (int i = 0; i < MAX_DISTRIBUTED_LOCKS; i++) {
        if (lock_manager_table[i].locked && lock_manager_table[i].lock_id == lock_id) {
            if (lock_manager_table[i].owner_node_id != node_id) {
                return -4; // Unauthorized release attempt
            }
            lock_manager_table[i].locked = 0;
            lock_manager_table[i].owner_node_id = 0;
            return 0;
        }
    }
    return -5; // Lock ID not found
}
