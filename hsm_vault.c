#include <stdint.h>
#include <stddef.h>

#define MAX_HSM_KEYS 16
#define KEY_DATA_LEN 32

typedef enum {
    KEY_TYPE_AES_256 = 1,
    KEY_TYPE_RSA_4096,
    KEY_TYPE_ED25519
} hsm_key_type_t;

typedef struct {
    uint32_t key_id;
    hsm_key_type_t type;
    uint8_t key_bytes[KEY_DATA_LEN];
    uint32_t usage_flags; // 1 = Sign, 2 = Verify, 4 = Encrypt, 8 = Decrypt
    uint8_t active;
} hsm_key_slot_t;

static hsm_key_slot_t hsm_vault_registry[MAX_HSM_KEYS];
static uint32_t next_hsm_key_id = 6000;

void hsm_vault_init(void) {
    for (int i = 0; i < MAX_HSM_KEYS; i++) {
        hsm_vault_registry[i].active = 0;
        hsm_vault_registry[i].usage_flags = 0;
    }
}

int hsm_vault_generate_key(hsm_key_type_t type, uint32_t usage_flags, uint32_t *out_key_id) {
    if (!out_key_id) return -1;

    for (int i = 0; i < MAX_HSM_KEYS; i++) {
        if (!hsm_vault_registry[i].active) {
            hsm_key_slot_t *slot = &hsm_vault_registry[i];
            slot->key_id = next_hsm_key_id++;
            slot->type = type;
            slot->usage_flags = usage_flags;

            // Simulate hardware entropy source for secure key generation
            for (int j = 0; j < KEY_DATA_LEN; j++) {
                slot->key_bytes[j] = (uint8_t)((slot->key_id + j) * 31 ^ 0x5A);
            }

            slot->active = 1;
            *out_key_id = slot->key_id;
            return 0;
        }
    }
    return -2; // HSM vault key slots full
}

int hsm_vault_sign_payload(uint32_t key_id, const uint8_t *payload, size_t size, uint8_t *out_signature) {
    if (!payload || size == 0 || !out_signature) return -1;

    for (int i = 0; i < MAX_HSM_KEYS; i++) {
        if (hsm_vault_registry[i].active && hsm_vault_registry[i].key_id == key_id) {
            hsm_key_slot_t *slot = &hsm_vault_registry[i];

            if (!(slot->usage_flags & 1)) {
                return -3; // Key not permitted for signing operations
            }

            // Simulate hardware cryptographic signing
            for (int j = 0; j < KEY_DATA_LEN; j++) {
                out_signature[j] = payload[j % size] ^ slot->key_bytes[j];
            }
            return 0;
        }
    }
    return -4; // Key ID not found in HSM vault
}
