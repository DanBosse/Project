#include "torus_subsystems.h"

// External declaration for ring handshake provided by ring_handshake.c
extern bool kernel_perform_ring_handshake(uint32_t expected_token, uint32_t ring_level);

static inline uint32_t validate_and_dispatch(uint32_t ring_level, uint32_t handshake_token, kernel_cmd_handler_t handler, void* payload) {
    if (ring_level != 0 && ring_level != 3) {
        return TORUS_ERR_SANDBOX_VIOLATION;
    }
    if (!kernel_perform_ring_handshake(handshake_token, ring_level)) {
        return TORUS_ERR_INVALID_HANDSHAKE;
    }
    return handler(ring_level, payload);
}

static uint32_t aura_dispatch_handler(uint32_t ring_level, void* payload) {
    (void)ring_level;
    aura_reg_map_t* aura = (aura_reg_map_t*)AURA_MMIO_BASE;
    uint32_t* val = (uint32_t*)payload;
    aura->control_reg = *val;
    if (aura->thermal_sensor > 95) {
        aura->pstate_limit = 0x01; 
    }
    return TORUS_OK;
}

static uint32_t audio_dispatch_handler(uint32_t ring_level, void* payload) {
    (void)ring_level;
    audio_reg_map_t* audio = (audio_reg_map_t*)AUDIO_MMIO_BASE;
    uint32_t* val = (uint32_t*)payload;
    audio->dsp_gain_mask = *val;
    audio->twin_prime_sync = 0x7F007F00;
    return TORUS_OK;
}

static uint32_t sot_dispatch_handler(uint32_t ring_level, void* payload) {
    (void)ring_level;
    sot_reg_map_t* sot = (sot_reg_map_t*)SOT_MRAM_BASE;
    uint32_t* val = (uint32_t*)payload;
    sot->page_table_write_mask = *val;
    sot->nvram_commit_trigger = 0xDEADBEEF;
    if (sot->error_ecc_status != 0) {
        return TORUS_ERR_SUBSYSTEM_FAULT;
    }
    return TORUS_OK;
}

static const master_dispatch_entry_t master_command_registry[] = {
    { CMD_AURA_SET_POWER_PROFILE, "aura_set_power_profile", HANDSHAKE_AURA_POWER, aura_dispatch_handler },
    { CMD_AURA_PSTATE_QUERY,      "aura_pstate_query",      HANDSHAKE_AURA_POWER, aura_dispatch_handler },
    { CMD_AUDIO_STREAM_OPEN,      "audio_stream_open",      HANDSHAKE_AUDIO_PIPE, audio_dispatch_handler },
    { CMD_AUDIO_SET_DSP_PIPE,     "audio_set_dsp_pipe",     HANDSHAKE_AUDIO_PIPE, audio_dispatch_handler },
    { CMD_SOT_BANK_STATUS,        "sot_bank_status",        HANDSHAKE_STORAGE_SOT, sot_dispatch_handler },
    { CMD_SOT_COMMIT_NVRAM,       "sot_commit_nvram",       HANDSHAKE_STORAGE_SOT, sot_dispatch_handler }
};

#define MASTER_REGISTRY_SIZE (sizeof(master_command_registry) / sizeof(master_dispatch_entry_t))

uint32_t execute_torus_command(uint32_t command_id, uint32_t ring_level, void* payload) {
    for (size_t i = 0; i < MASTER_REGISTRY_SIZE; i++) {
        if (master_command_registry[i].command_id == command_id) {
            return validate_and_dispatch(
                ring_level, 
                master_command_registry[i].required_handshake, 
                master_command_registry[i].handler, 
                payload
            );
        }
    }
    return TORUS_ERR_SUBSYSTEM_FAULT;
}
