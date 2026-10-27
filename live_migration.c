#include <stdint.h>
#include <stddef.h>

#define MAX_MIGRATION_SESSIONS 16
#define MIGRATION_CHUNK_SIZE 1024

typedef enum {
    MIG_STATE_INIT = 0,
    MIG_STATE_PRECOPY,
    MIG_STATE_SUSPEND_AND_TRANSFER,
    MIG_STATE_COMPLETED,
    MIG_STATE_FAILED
} migration_state_t;

typedef struct {
    uint32_t session_id;
    uint32_t vmid;
    uint32_t source_node_id;
    uint32_t target_node_id;
    migration_state_t state;
    uint32_t bytes_transferred;
    uint32_t total_bytes;
    uint8_t active;
} migration_session_t;

static migration_session_t migration_registry[MAX_MIGRATION_SESSIONS];
static uint32_t next_mig_session_id = 500000;

void live_migration_init(void) {
    for (int i = 0; i < MAX_MIGRATION_SESSIONS; i++) {
        migration_registry[i].active = 0;
        migration_registry[i].state = MIG_STATE_INIT;
    }
}

int live_migration_start(uint32_t vmid, uint32_t src_node, uint32_t target_node, uint32_t total_memory_bytes, uint32_t *out_session_id) {
    if (!out_session_id || total_memory_bytes == 0) return -1;

    for (int i = 0; i < MAX_MIGRATION_SESSIONS; i++) {
        if (!migration_registry[i].active) {
            migration_session_t *sess = &migration_registry[i];
            sess->session_id = next_mig_session_id++;
            sess->vmid = vmid;
            sess->source_node_id = src_node;
            sess->target_node_id = target_node;
            sess->state = MIG_STATE_PRECOPY;
            sess->bytes_transferred = 0;
            sess->total_bytes = total_memory_bytes;
            sess->active = 1;

            *out_session_id = sess->session_id;
            return 0;
        }
    }
    return -2; // Migration session registry full
}

int live_migration_step(uint32_t session_id) {
    for (int i = 0; i < MAX_MIGRATION_SESSIONS; i++) {
        if (migration_registry[i].active && migration_registry[i].session_id == session_id) {
            migration_session_t *sess = &migration_registry[i];

            if (sess->state == MIG_STATE_COMPLETED || sess->state == MIG_STATE_FAILED) {
                return -3; // Migration already finished
            }

            if (sess->bytes_transferred < sess->total_bytes) {
                sess->bytes_transferred += MIGRATION_CHUNK_SIZE;
                if (sess->bytes_transferred >= sess->total_bytes) {
                    sess->bytes_transferred = sess->total_bytes;
                    sess->state = MIG_STATE_SUSPEND_AND_TRANSFER;
                }
            } else {
                sess->state = MIG_STATE_COMPLETED;
            }
            return 0;
        }
    }
    return -4; // Session ID not found
}
