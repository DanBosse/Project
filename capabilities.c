#include <stdint.h>
#include <stddef.h>

#define MAX_CAPABILITIES 64

typedef enum {
    CAP_RIGHT_READ = 1 << 0,
    CAP_RIGHT_WRITE = 1 << 1,
    CAP_RIGHT_EXECUTE = 1 << 2,
    CAP_RIGHT_DELEGATE = 1 << 3,
    CAP_RIGHT_ADMIN = 1 << 4
} capability_rights_t;

typedef struct {
    uint32_t token_id;
    uint32_t owner_tier;
    uint32_t resource_id;
    uint32_t rights_mask;
    uint8_t active;
} capability_token_t;

static capability_token_t capability_table[MAX_CAPABILITIES];
static uint32_t next_token_id = 900000;

void capabilities_init(void) {
    for (int i = 0; i < MAX_CAPABILITIES; i++) {
        capability_table[i].active = 0;
        capability_table[i].rights_mask = 0;
    }
}

int capabilities_mint(uint32_t owner_tier, uint32_t resource_id, uint32_t rights, uint32_t *out_token_id) {
    if (!out_token_id || owner_tier == 0 || owner_tier > 6) return -1;

    for (int i = 0; i < MAX_CAPABILITIES; i++) {
        if (!capability_table[i].active) {
            capability_token_t *cap = &capability_table[i];
            cap->token_id = next_token_id++;
            cap->owner_tier = owner_tier;
            cap->resource_id = resource_id;
            cap->rights_mask = rights;
            cap->active = 1;

            *out_token_id = cap->token_id;
            return 0;
        }
    }
    return -2; // Capability table full
}

int capabilities_validate(uint32_t token_id, uint32_t required_rights, uint32_t accessor_tier) {
    for (int i = 0; i < MAX_CAPABILITIES; i++) {
        if (capability_table[i].active && capability_table[i].token_id == token_id) {
            capability_token_t *cap = &capability_table[i];

            if (cap->owner_tier != accessor_tier) {
                return -3; // Tier mismatch (unauthorized accessor)
            }

            if ((cap->rights_mask & required_rights) != required_rights) {
                return -4; // Insufficient capability rights
            }

            return 0; // Access granted
        }
    }
    return -5; // Token ID not found or inactive
}
