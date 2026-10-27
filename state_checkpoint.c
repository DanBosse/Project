#include <stdint.h>
#include <stddef.h>

#define MAX_CHECKPOINTS 8
#define CHECKPOINT_STATE_SIZE 1024

typedef struct {
    uint32_t checkpoint_id;
    uint64_t timestamp;
    uint8_t data[CHECKPOINT_STATE_SIZE];
    uint32_t checksum;
    uint8_t active;
} system_checkpoint_t;

static system_checkpoint_t checkpoint_store[MAX_CHECKPOINTS];
static uint32_t active_checkpoint_index = 0;

void state_checkpoint_init(void) {
    for (int i = 0; i < MAX_CHECKPOINTS; i++) {
        checkpoint_store[i].active = 0;
    }
}

int state_checkpoint_create(uint32_t cp_id, const uint8_t *state_data, size_t size) {
    if (!state_data || size > CHECKPOINT_STATE_SIZE) return -1;
    
    for (int i = 0; i < MAX_CHECKPOINTS; i++) {
        if (!checkpoint_store[i].active) {
            checkpoint_store[i].checkpoint_id = cp_id;
            checkpoint_store[i].timestamp = 1710000000; // Epoch reference stub
            
            uint32_t sum = 0;
            for (size_t j = 0; j < size; j++) {
                checkpoint_store[i].data[j] = state_data[j];
                sum += state_data[j];
            }
            checkpoint_store[i].checksum = sum;
            checkpoint_store[i].active = 1;
            active_checkpoint_index = i;
            return 0;
        }
    }
    return -2; // Checkpoint storage full
}

int state_checkpoint_rollback(uint32_t cp_id, uint8_t *out_state) {
    for (int i = 0; i < MAX_CHECKPOINTS; i++) {
        if (checkpoint_store[i].active && checkpoint_store[i].checkpoint_id == cp_id) {
            if (out_state) {
                for (size_t j = 0; j < CHECKPOINT_STATE_SIZE; j++) {
                    out_state[j] = checkpoint_store[i].data[j];
                }
            }
            active_checkpoint_index = i;
            return 0;
        }
    }
    return -3; // Checkpoint ID not found
}
