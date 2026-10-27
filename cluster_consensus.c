#include <stdint.h>
#include <stddef.h>

typedef enum {
    NODE_STATE_FOLLOWER = 0,
    NODE_STATE_CANDIDATE,
    NODE_STATE_LEADER
} raft_node_state_t;

typedef struct {
    uint32_t term;
    uint32_t command_id;
    uint64_t data_payload;
} raft_log_entry_t;

#define MAX_RAFT_LOGS 128

typedef struct {
    uint32_t node_id;
    raft_node_state_t state;
    uint32_t current_term;
    uint32_t voted_for;
    raft_log_entry_t log[MAX_RAFT_LOGS];
    uint32_t log_count;
    uint32_t commit_index;
} cluster_consensus_node_t;

static cluster_consensus_node_t local_consensus_node;

void cluster_consensus_init(uint32_t node_id) {
    local_consensus_node.node_id = node_id;
    local_consensus_node.state = NODE_STATE_FOLLOWER;
    local_consensus_node.current_term = 1;
    local_consensus_node.voted_for = 0;
    local_consensus_node.log_count = 0;
    local_consensus_node.commit_index = 0;
}

int cluster_consensus_append_log(uint32_t command_id, uint64_t payload) {
    if (local_consensus_node.log_count >= MAX_RAFT_LOGS) {
        return -1; // Raft log storage full
    }
    
    uint32_t idx = local_consensus_node.log_count++;
    local_consensus_node.log[idx].term = local_consensus_node.current_term;
    local_consensus_node.log[idx].command_id = command_id;
    local_consensus_node.log[idx].data_payload = payload;
    return (int)idx;
}

int cluster_consensus_request_vote(uint32_t candidate_id, uint32_t term) {
    if (term > local_consensus_node.current_term) {
        local_consensus_node.current_term = term;
        local_consensus_node.state = NODE_STATE_FOLLOWER;
        local_consensus_node.voted_for = candidate_id;
        return 1; // Vote granted
    }
    return 0; // Vote denied
}
