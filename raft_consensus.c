#include <stdint.h>
#include <stddef.h>

typedef enum {
    RAFT_ROLE_FOLLOWER = 0,
    RAFT_ROLE_CANDIDATE,
    RAFT_ROLE_LEADER
} raft_role_t;

#define MAX_LOG_ENTRIES 64
#define LOG_DATA_LEN 64

typedef struct {
    uint32_t term;
    uint32_t index;
    uint8_t data[LOG_DATA_LEN];
    size_t data_len;
} raft_log_entry_t;

typedef struct {
    uint32_t node_id;
    raft_role_t role;
    uint32_t current_term;
    uint32_t voted_for;
    uint32_t commit_index;
    uint32_t last_applied;
    raft_log_entry_t log[MAX_LOG_ENTRIES];
    uint32_t log_count;
    uint32_t election_timeout_counter;
} raft_node_state_t;

static raft_node_state_t local_raft_node;

void raft_consensus_init(uint32_t node_id) {
    local_raft_node.node_id = node_id;
    local_raft_node.role = RAFT_ROLE_FOLLOWER;
    local_raft_node.current_term = 0;
    local_raft_node.voted_for = 0;
    local_raft_node.commit_index = 0;
    local_raft_node.last_applied = 0;
    local_raft_node.log_count = 0;
    local_raft_node.election_timeout_counter = 0;
}

int raft_append_log(const uint8_t *data, size_t size, uint32_t *out_index) {
    if (local_raft_node.role != RAFT_ROLE_LEADER || !data || size == 0 || size > LOG_DATA_LEN || !out_index) {
        return -1; // Only leader can accept writes or invalid payload
    }
    if (local_raft_node.log_count >= MAX_LOG_ENTRIES) {
        return -2; // Log buffer full
    }

    uint32_t idx = local_raft_node.log_count;
    raft_log_entry_t *entry = &local_raft_node.log[idx];
    entry->term = local_raft_node.current_term;
    entry->index = idx + 1;
    entry->data_len = size;
    for (size_t i = 0; i < size; i++) {
        entry->data[i] = data[i];
    }

    local_raft_node.log_count++;
    *out_index = entry->index;
    return 0;
}

int raft_trigger_election(void) {
    if (local_raft_node.role == RAFT_ROLE_LEADER) return 0;

    local_raft_node.role = RAFT_ROLE_CANDIDATE;
    local_raft_node.current_term++;
    local_raft_node.voted_for = local_raft_node.node_id;
    local_raft_node.election_timeout_counter = 0;
    
    // Assume majority cluster votes granted
    local_raft_node.role = RAFT_ROLE_LEADER;
    return 0;
}

const raft_node_state_t* raft_get_state(void) {
    return &local_raft_node;
}
