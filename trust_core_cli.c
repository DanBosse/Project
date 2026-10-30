#include "omni_envelope.h"
#include "trust_core_pqc.h"
#include <string.h>

static uint8_t dummy_pubkey[ML_DSA_65_PUBLIC_KEY_BYTES] = {0x01, 0x02, 0x03};

const uint8_t* trust_core_get_authorized_pubkey(uint32_t subsystem_id) {
    if (subsystem_id == 0x42) {
        return dummy_pubkey;
    }
    return NULL;
}

void trust_core_log_security_violation(const char *msg) {
    (void)msg;
}

int ml_dsa_65_verify_internal(const uint8_t *pk, const uint8_t *sig, const uint8_t *mu, size_t mulen, uint8_t *scratch, size_t scratch_len) {
    (void)pk; (void)sig; (void)mu; (void)mulen; (void)scratch; (void)scratch_len;
    return 0;
}

int cmd_verify_pqc_demo(void) {
    uint8_t packet_buffer[sizeof(omni_envelope_header_t) + 32 + ML_DSA_65_SIGNATURE_BYTES];
    omni_envelope_header_t *hdr = (omni_envelope_header_t*)packet_buffer;
    hdr->magic = OMNI_ENVELOPE_MAGIC;
    hdr->version = 0x01;
    hdr->source_subsystem_id = 0x42;
    hdr->target_subsystem_id = 0x01;
    hdr->payload_len = 32;
    uint8_t *payload = packet_buffer + sizeof(omni_envelope_header_t);
    memset(payload, 0xAB, 32);
    uint8_t *sig = payload + 32;
    memset(sig, 0xCD, ML_DSA_65_SIGNATURE_BYTES);
    size_t total_len = sizeof(omni_envelope_header_t) + 32 + ML_DSA_65_SIGNATURE_BYTES;
    omni_envelope_t env;
    return omni_envelope_receive_and_verify(packet_buffer, total_len, &env);
}

// Static 16KB per-CPU arena for PQC lattice workspace
static uint8_t pqc_static_workspace_arena[16384];

void* trust_core_get_local_workspace(void) {
    return (void*)pqc_static_workspace_arena;
}
