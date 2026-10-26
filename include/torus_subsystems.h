#ifndef TORUS_SUBSYSTEMS_H
#define TORUS_SUBSYSTEMS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define TORUS_OK                      0x00000000
#define TORUS_ERR_SANDBOX_VIOLATION   0xE0000001
#define TORUS_ERR_INVALID_HANDSHAKE   0xE0000002
#define TORUS_ERR_SUBSYSTEM_FAULT     0xE0000003

// Subsystem Handshake Identifiers
#define HANDSHAKE_AURA_POWER          0x1B
#define HANDSHAKE_AUDIO_PIPE          0x2C
#define HANDSHAKE_STORAGE_SOT         0x3F

// Higher-Half MMIO & SOT-MRAM Base Addresses
#define AURA_MMIO_BASE                0xFFFF800000010000ULL
#define AUDIO_MMIO_BASE               0xFFFF800000020000ULL
#define SOT_MRAM_BASE                 0xFFFF800000030000ULL

// Command IDs
#define CMD_AURA_SET_POWER_PROFILE    0xA001
#define CMD_AURA_PSTATE_QUERY         0xA002
#define CMD_AUDIO_STREAM_OPEN         0xB001
#define CMD_AUDIO_SET_DSP_PIPE        0xB002
#define CMD_SOT_BANK_STATUS           0xC001
#define CMD_SOT_COMMIT_NVRAM          0xC002

// Telemetry & Register Structures
typedef struct {
    volatile uint32_t control_reg;
    volatile uint32_t pstate_limit;
    volatile uint32_t thermal_sensor;
    volatile uint32_t telemetry_flags;
} __attribute__((packed)) aura_reg_map_t;

typedef struct {
    volatile uint32_t ring_buffer_head;
    volatile uint32_t ring_buffer_tail;
    volatile uint32_t dsp_gain_mask;
    volatile uint32_t twin_prime_sync;
} __attribute__((packed)) audio_reg_map_t;

typedef struct {
    volatile uint32_t bank_status_mask;
    volatile uint32_t nvram_commit_trigger;
    volatile uint32_t page_table_write_mask;
    volatile uint32_t error_ecc_status;
} __attribute__((packed)) sot_reg_map_t;

// Function Pointer Type for Unified Handlers
typedef uint32_t (*kernel_cmd_handler_t)(uint32_t ring_level, void* envelope_payload);

typedef struct {
    uint32_t command_id;
    const char* command_name;
    uint32_t required_handshake;
    kernel_cmd_handler_t handler;
} __attribute__((packed)) master_dispatch_entry_t;

// Core Dispatch API
uint32_t execute_torus_command(uint32_t command_id, uint32_t ring_level, void* payload);
bool kernel_perform_ring_handshake(uint32_t expected_token, uint32_t ring_level);

#endif // TORUS_SUBSYSTEMS_H
