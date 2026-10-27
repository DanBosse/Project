#include <stdint.h>
#include <stddef.h>

#define MAX_PEER_NODES 16
#define NODE_IP_LEN 16

typedef struct {
    uint32_t node_id;
    char ip_address[NODE_IP_LEN];
    uint64_t last_sync_tick;
    uint32_t active_workloads;
    uint8_t connected;
} peer_node_t;

typedef struct {
    peer_node_t peers[MAX_PEER_NODES];
    uint32_t peer_count;
    uint32_t cluster_id;
} federation_registry_t;

static federation_registry_t cluster_federation;

void node_federation_init(void) {
    cluster_federation.peer_count = 0;
    cluster_federation.cluster_id = 0x7A4F8821; // Sovereign cluster magic identifier
    for (int i = 0; i < MAX_PEER_NODES; i++) {
        cluster_federation.peers[i].connected = 0;
    }
}

int node_federation_add_peer(uint32_t node_id, const char *ip) {
    if (!ip || cluster_federation.peer_count >= MAX_PEER_NODES) return -1;
    
    peer_node_t *peer = &cluster_federation.peers[cluster_federation.peer_count];
    peer->node_id = node_id;
    int i = 0;
    while (ip[i] != '\0' && i < NODE_IP_LEN - 1) {
        peer->ip_address[i] = ip[i];
        i++;
    }
    peer->ip_address[i] = '\0';
    peer->last_sync_tick = 0;
    peer->active_workloads = 0;
    peer->connected = 1;
    
    cluster_federation.peer_count++;
    return 0;
}

int node_federation_sync_heartbeat(uint32_t node_id, uint64_t current_tick) {
    for (int i = 0; i < MAX_PEER_NODES; i++) {
        if (cluster_federation.peers[i].connected && cluster_federation.peers[i].node_id == node_id) {
            cluster_federation.peers[i].last_sync_tick = current_tick;
            return 0;
        }
    }
    return -2; // Peer node not found in federation mesh
}
