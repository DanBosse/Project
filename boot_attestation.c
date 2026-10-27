#include <stdint.h>
#include <stddef.h>

#define MAX_PCR_BANKS 8
#define PCR_HASH_LEN 32

typedef struct {
    uint32_t pcr_index;
    uint8_t digest[PCR_HASH_LEN];
    uint32_t extend_count;
    uint8_t locked;
} pcr_register_t;

static pcr_register_t pcr_bank_registry[MAX_PCR_BANKS];

void boot_attestation_init(void) {
    for (int i = 0; i < MAX_PCR_BANKS; i++) {
        pcr_bank_registry[i].pcr_index = i;
        pcr_bank_registry[i].extend_count = 0;
        pcr_bank_registry[i].locked = 0;

        // Initialize PCR digest to zero state
        for (int j = 0; j < PCR_HASH_LEN; j++) {
            pcr_bank_registry[i].digest[j] = 0x00;
        }
    }
}

int boot_attestation_extend_pcr(uint32_t pcr_index, const uint8_t *measurement_data, size_t size) {
    if (pcr_index >= MAX_PCR_BANKS || !measurement_data || size == 0) {
        return -1; // Invalid PCR index or measurement payload
    }

    pcr_register_t *pcr = &pcr_bank_registry[pcr_index];

    if (pcr->locked) {
        return -2; // PCR register locked post-boot
    }

    // Simulate cryptographic extension: Digest_new = Hash(Digest_old || Measurement)
    for (int j = 0; j < PCR_HASH_LEN; j++) {
        uint8_t mix = measurement_data[j % size];
        pcr->digest[j] = (pcr->digest[j] ^ mix) + (uint8_t)(pcr->extend_count + j);
    }

    pcr->extend_count++;
    return 0;
}

int boot_attestation_verify_baseline(uint32_t pcr_index, const uint8_t *expected_digest) {
    if (pcr_index >= MAX_PCR_BANKS || !expected_digest) {
        return -1;
    }

    pcr_register_t *pcr = &pcr_bank_registry[pcr_index];

    // Compare measured PCR state against expected gold-standard baseline
    for (int j = 0; j < PCR_HASH_LEN; j++) {
        if (pcr->digest[j] != expected_digest[j]) {
            return -3; // Attestation failure: PCR baseline mismatch detected
        }
    }

    // Lock PCR post-verification to ensure tamper resistance
    pcr->locked = 1;
    return 0; // Attestation verified successfully
}
