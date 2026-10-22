#ifndef STUDIO_CORE_H
#define STUDIO_CORE_H

#include <stdint.h>
#include <stdbool.h>

#define STUDIO_PIPE_CHANNELS 4
#define STUDIO_PIPE_BUFFER_SIZE 256

typedef struct {
    uint32_t source_studio_id;
    uint32_t target_studio_id;
    float metrics[8];
    char label[32];
    bool active;
} StudioPipeChannel;

// Universal Studio Core Initialization
void studio_core_init(void);

// Universal Command Dispatcher available inside any studio
bool studio_core_dispatch(const char* cmd_buffer);

// Cross-subsystem data piping & shared buffers
void* studio_core_get_shared_buffer(uint32_t channel);
void studio_core_set_shared_buffer(uint32_t channel, void* ptr);

// AuraLink Real-Time Data Piping API
bool studio_core_write_pipe(uint32_t channel_id, const float* data, uint32_t count, const char* label);
bool studio_core_read_pipe(uint32_t channel_id, float* out_data, uint32_t* count, char* out_label);

#endif
