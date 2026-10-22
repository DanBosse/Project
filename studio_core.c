#include "studio_core.h"
#include "torus.h"

// Shared inter-op memory channels across labs
static void* shared_channels[8] = { 0 };
static StudioPipeChannel active_pipes[STUDIO_PIPE_CHANNELS];

void studio_core_init(void) {
    serial_print("[StudioCore] Initializing Universal Inter-Op & Command Router...\n");
    for (int i = 0; i < 8; i++) {
        shared_channels[i] = (void*)0;
    }
    for (int i = 0; i < STUDIO_PIPE_CHANNELS; i++) {
        active_pipes[i].active = false;
        active_pipes[i].source_studio_id = 0;
        active_pipes[i].target_studio_id = 0;
        for (int m = 0; m < 8; m++) active_pipes[i].metrics[m] = 0.0f;
        active_pipes[i].label[0] = '\0';
    }
}

// Lightweight string comparison helper
static int sc_streq(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

bool studio_core_dispatch(const char* cmd_buffer) {
    if (cmd_buffer == (void*)0 || cmd_buffer[0] == '\0') {
        return false;
    }

    serial_print("[StudioCore] Dispatching command from active studio: ");
    serial_print(cmd_buffer);
    serial_print("\n");

    if (sc_streq(cmd_buffer, "dashboard") == 0) {
        serial_print("[StudioCore] Routing to StudioLens Telemetry Dashboard...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "videostudio") == 0) {
        serial_print("[StudioCore] Switching workspace to Video NLE Suite...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "audiostudio") == 0) {
        serial_print("[StudioCore] Switching workspace to Audio DAW & Mixer...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "graphicsstudio") == 0) {
        serial_print("[StudioCore] Switching workspace to Graphics & Design Canvas...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "physicslab") == 0) {
        serial_print("[StudioCore] Switching workspace to AuraSpring Physics Lab...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "aura") == 0) {
        serial_print("[StudioCore] Launching Aura AI Core from active studio workspace...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "auratest") == 0) {
        serial_print("[StudioCore] Running Aura AI & Transformer Benchmark...\n");
        return true;
    } else if (sc_streq(cmd_buffer, "auralink") == 0) {
        serial_print("[StudioCore] Verifying AuraLink cross-subsystem pipes...\n");
        for (int i = 0; i < STUDIO_PIPE_CHANNELS; i++) {
            if (active_pipes[i].active) {
                serial_print("  [Pipe Active] Channel active with label: ");
                serial_print(active_pipes[i].label);
                serial_print("\n");
            }
        }
        return true;
    } else if (sc_streq(cmd_buffer, "help") == 0) {
        serial_print("[StudioCore] Universal Studio Commands:\n");
        serial_print("  - videostudio, audiostudio, graphicsstudio, physicslab\n");
        serial_print("  - aura, auratest, dashboard, status, auralink, clear\n");
        return true;
    }

    serial_print("[StudioCore] Unknown cross-studio command. Pass-through to kernel.\n");
    return false;
}

void* studio_core_get_shared_buffer(uint32_t channel) {
    if (channel < 8) return shared_channels[channel];
    return (void*)0;
}

void studio_core_set_shared_buffer(uint32_t channel, void* ptr) {
    if (channel < 8) {
        shared_channels[channel] = ptr;
    }
}

bool studio_core_write_pipe(uint32_t channel_id, const float* data, uint32_t count, const char* label) {
    if (channel_id >= STUDIO_PIPE_CHANNELS || data == (void*)0) return false;
    
    active_pipes[channel_id].active = true;
    uint32_t limit = count > 8 ? 8 : count;
    for (uint32_t i = 0; i < limit; i++) {
        active_pipes[channel_id].metrics[i] = data[i];
    }
    if (label != (void*)0) {
        int idx = 0;
        while (label[idx] != '\0' && idx < 31) {
            active_pipes[channel_id].label[idx] = label[idx];
            idx++;
        }
        active_pipes[channel_id].label[idx] = '\0';
    }
    return true;
}

bool studio_core_read_pipe(uint32_t channel_id, float* out_data, uint32_t* count, char* out_label) {
    if (channel_id >= STUDIO_PIPE_CHANNELS || !active_pipes[channel_id].active) return false;

    if (count != (void*)0) {
        *count = 8;
        if (out_data != (void*)0) {
            for (int i = 0; i < 8; i++) {
                out_data[i] = active_pipes[channel_id].metrics[i];
            }
        }
    }
    if (out_label != (void*)0) {
        int idx = 0;
        while (active_pipes[channel_id].label[idx] != '\0' && idx < 31) {
            out_label[idx] = active_pipes[channel_id].label[idx];
            idx++;
        }
        out_label[idx] = '\0';
    }
    return true;
}
