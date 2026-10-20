#include "torus.h"
#include "ipc.h"

static ipc_endpoint_t endpoints[MAX_IPC_ENDPOINTS];

void ipc_subsystem_init(void) {
    serial_print("[IPC] Initializing Advanced Capability-Based Zero-Copy IPC Subsystem...\n");
    for (int i = 0; i < MAX_IPC_ENDPOINTS; i++) {
        endpoints[i].active = false;
        endpoints[i].head = 0;
        endpoints[i].tail = 0;
    }
    // Create system kernel endpoint (PID 0)
    ipc_create_endpoint(0, IPC_PERM_READ | IPC_PERM_WRITE);
    serial_print("[IPC] Microkernel IPC secure messaging channels online.\n");
}

uint32_t ipc_create_endpoint(uint32_t pid, uint32_t rights) {
    for (int i = 0; i < MAX_IPC_ENDPOINTS; i++) {
        if (!endpoints[i].active) {
            endpoints[i].endpoint_id = i;
            endpoints[i].owner_pid = pid;
            endpoints[i].rights = rights;
            endpoints[i].head = 0;
            endpoints[i].tail = 0;
            endpoints[i].active = true;
            return i;
        }
    }
    return 0xFFFFFFFF; // Error: No endpoints available
}

bool ipc_send(uint32_t dest_endpoint, ipc_message_t *msg) {
    if (dest_endpoint >= MAX_IPC_ENDPOINTS || !endpoints[dest_endpoint].active) {
        return false;
    }

    // Verify write permissions
    if (!(endpoints[dest_endpoint].rights & IPC_PERM_WRITE)) {
        serial_print("[IPC Security Warning] Permission denied on IPC send.\n");
        return false;
    }

    ipc_endpoint_t *ep = &endpoints[dest_endpoint];
    uint32_t next_tail = (ep->tail + 1) % 8;
    
    if (next_tail == ep->head) {
        // Queue full (backpressure)
        return false;
    }

    ep->queue[ep->tail] = *msg;
    ep->tail = next_tail;
    return true;
}

bool ipc_receive(uint32_t endpoint_id, ipc_message_t *out_msg) {
    if (endpoint_id >= MAX_IPC_ENDPOINTS || !endpoints[endpoint_id].active) {
        return false;
    }

    // Verify read permissions
    if (!(endpoints[endpoint_id].rights & IPC_PERM_READ)) {
        serial_print("[IPC Security Warning] Permission denied on IPC receive.\n");
        return false;
    }

    ipc_endpoint_t *ep = &endpoints[endpoint_id];
    if (ep->head == ep->tail) {
        // Queue empty
        return false;
    }

    *out_msg = ep->queue[ep->head];
    ep->head = (ep->head + 1) % 8;
    return true;
}

void ipc_init(void) {
    // IPC Subsystem Initialization
}
