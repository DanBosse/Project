#include <stddef.h>
#include <stdint.h>
#include "kernel_utils.h"
#include "aura.h"
#include "enterprise_telemetry.h"

// Fallback definitions if headers lack them
#ifndef IPC_CHANNEL_USER_TERMINAL
#define IPC_CHANNEL_USER_TERMINAL 0x01
#endif

void neural_ipc_dispatch_packet(uint32_t channel_id, void* buffer, size_t len) {
    if (channel_id != IPC_CHANNEL_USER_TERMINAL) {
        kernel_print("[NEURAL_IPC] Error: Unauthorized IPC channel access attempt.\n");
        return;
    }

    (void)buffer;
    (void)len;
    kernel_print("[NEURAL_IPC] Processing remote terminal payload from Ring 3 sandbox...\n");

    #ifdef enterprise_telemetry_poll
    telemetry_snapshot_t snap;
    enterprise_telemetry_poll(&snap);
    aura_broadcast_ipc_packet(channel_id, &snap, sizeof(snap));
    #endif
}
