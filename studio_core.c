#include <stddef.h>
#include "torus.h"
#include "studio_core.h"
#include "studiolens_dashboard.h"

// External reference to the master registry dispatcher
extern void shell_dispatch_command(const char *cmd_line);

// Shared buffer and pipe state storage
static void* shared_buffers[STUDIO_PIPE_CHANNELS];
static StudioPipeChannel pipe_channels[STUDIO_PIPE_CHANNELS];

static bool string_equals(const char *s1, const char *s2) {
    if (!s1 || !s2) return false;
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 == *(const unsigned char*)s2;
}

void studio_core_init(void) {
    for (int i = 0; i < STUDIO_PIPE_CHANNELS; i++) {
        shared_buffers[i] = NULL;
        pipe_channels[i].active = false;
    }
    serial_print("[StudioCore] Initializing Universal Inter-Op & Command Router...\n");
}

bool studio_core_dispatch(const char* cmd_buffer) {
    if (!cmd_buffer || *cmd_buffer == '\0') return true;

    serial_print("[StudioCore] Routing command to Master Registry: ");
    serial_print(cmd_buffer);
    serial_print("\n");

    // Intercept dashboard to render the StudioLens telemetry dashboard directly
    if (string_equals(cmd_buffer, "dashboard")) {
        studiolens_render_telemetry_dashboard();
        return true;
    }

    // Forward to master command registry dispatcher
    shell_dispatch_command(cmd_buffer);
    return true;
}

void* studio_core_get_shared_buffer(uint32_t channel) {
    if (channel >= STUDIO_PIPE_CHANNELS) return NULL;
    return shared_buffers[channel];
}

void studio_core_set_shared_buffer(uint32_t channel, void* ptr) {
    if (channel >= STUDIO_PIPE_CHANNELS) return;
    shared_buffers[channel] = ptr;
}

bool studio_core_write_pipe(uint32_t channel_id, const float* data, uint32_t count, const char* label) {
    if (channel_id >= STUDIO_PIPE_CHANNELS) return false;
    pipe_channels[channel_id].active = true;
    if (label) {
        int i = 0;
        while (label[i] && i < 31) {
            pipe_channels[channel_id].label[i] = label[i];
            i++;
        }
        pipe_channels[channel_id].label[i] = '\0';
    }
    for (uint32_t i = 0; i < count && i < 8; i++) {
        pipe_channels[channel_id].metrics[i] = data ? data[i] : 0.0f;
    }
    return true;
}

bool studio_core_read_pipe(uint32_t channel_id, float* out_data, uint32_t* count, char* out_label) {
    if (channel_id >= STUDIO_PIPE_CHANNELS || !pipe_channels[channel_id].active) return false;
    if (count) *count = 8;
    if (out_data) {
        for (int i = 0; i < 8; i++) {
            out_data[i] = pipe_channels[channel_id].metrics[i];
        }
    }
    if (out_label) {
        int i = 0;
        while (pipe_channels[channel_id].label[i] && i < 31) {
            out_label[i] = pipe_channels[channel_id].label[i];
            i++;
        }
        out_label[i] = '\0';
    }
    return true;
}
