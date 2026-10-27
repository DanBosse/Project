#include <stdint.h>
#include <stddef.h>

#define MAX_SHM_REGIONS 16
#define SHM_REGION_SIZE (4 * 1024 * 1024) // 4MB per region block

typedef struct {
    uint32_t region_id;
    uint8_t buffer[SHM_REGION_SIZE];
    size_t size;
    uint32_t owner_tier;
    uint8_t active;
} shm_region_t;

static shm_region_t shm_pool[MAX_SHM_REGIONS];

void zero_copy_shm_init(void) {
    for (int i = 0; i < MAX_SHM_REGIONS; i++) {
        shm_pool[i].active = 0;
        shm_pool[i].region_id = i;
        shm_pool[i].size = SHM_REGION_SIZE;
    }
}

int zero_copy_shm_alloc(uint32_t owner_tier, void **out_ptr) {
    if (!out_ptr) return -1;
    for (int i = 0; i < MAX_SHM_REGIONS; i++) {
        if (!shm_pool[i].active) {
            shm_pool[i].active = 1;
            shm_pool[i].owner_tier = owner_tier;
            *out_ptr = (void *)shm_pool[i].buffer;
            return i;
        }
    }
    return -2; // Out of shared memory regions
}

int zero_copy_shm_free(uint32_t region_id) {
    if (region_id >= MAX_SHM_REGIONS || !shm_pool[region_id].active) {
        return -1;
    }
    shm_pool[region_id].active = 0;
    return 0;
}
