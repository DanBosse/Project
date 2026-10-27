#include <stdint.h>
#include <stddef.h>

#define MAX_CAPABILITIES 64

typedef enum {
    CAP_PERMISSION_NONE = 0,
    CAP_PERMISSION_READ = 1 << 0,
    CAP_PERMISSION_WRITE = 1 << 1,
    CAP_PERMISSION_EXECUTE = 1 << 2,
    CAP_PERMISSION_HARDWARE_ACCEL = 1 << 3,
    CAP_PERMISSION_FULL_ACCESS = 0xFF
} cap_permissions_t;

typedef struct {
    uint32_t token_id;
    uint32_t owner_tier;
    uint32_t target_resource_id;
    uint32_t permissions;
    uint8_t active;
} capability_token_t;

static capability_token_t cap_registry[MAX_CAPABILITIES];
static uint32_t next_token_id = 1000;

void secure_capability_init(void) {
    for (int i = 0; i < MAX_CAPABILITIES; i++) {
        cap_registry[i].active = 0;
    }
}

int secure_capability_issue(uint32_t owner_tier, uint32_t target_resource, uint32_t permissions, uint32_t *out_token_id) {
    if (!out_token_id) return -1;
    for (int i = 0; i < MAX_CAPABILITIES; i++) {
        if (!cap_registry[i].active) {
            cap_registry[i].token_id = next_token_id++;
            cap_registry[i].owner_tier = owner_tier;
            cap_registry[i].target_resource_id = target_resource;
            cap_registry[i].permissions = permissions;
            cap_registry[i].active = 1;
            *out_token_id = cap_registry[i].token_id;
            return 0;
        }
    }
    return -2; // Capability registry full
}

int secure_capability_verify(uint32_t token_id, uint32_t required_permissions) {
    for (int i = 0; i < MAX_CAPABILITIES; i++) {
        if (cap_registry[i].active && cap_registry[i].token_id == token_id) {
            if ((cap_registry[i].permissions & required_permissions) == required_permissions) {
                return 0; // Authorized
            }
            return -3; // Insufficient permissions
        }
    }
    return -4; // Invalid capability token
}
