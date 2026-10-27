#include <stdint.h>
#include <stddef.h>

#define MAX_ACCELERATORS 8
#define ACCEL_MEM_POOL_SIZE (32 * 1024 * 1024) // 32MB zero-copy shared buffer per device

typedef enum {
    ACCEL_TYPE_GPU = 0,
    ACCEL_TYPE_NPU,
    ACCEL_TYPE_VPU,
    ACCEL_TYPE_DPU
} accelerator_type_t;

typedef struct {
    uint32_t device_id;
    accelerator_type_t type;
    uint8_t shared_memory_pool[ACCEL_MEM_POOL_SIZE];
    size_t allocated_bytes;
    uint32_t active_jobs_count;
    uint8_t active;
} accelerator_device_t;

static accelerator_device_t accelerator_registry[MAX_ACCELERATORS];
static uint32_t next_accel_device_id = 3000;

void accelerator_subsystem_init(void) {
    for (int i = 0; i < MAX_ACCELERATORS; i++) {
        accelerator_registry[i].active = 0;
        accelerator_registry[i].allocated_bytes = 0;
        accelerator_registry[i].active_jobs_count = 0;
    }
}

int accelerator_register_device(accelerator_type_t type, uint32_t *out_device_id) {
    if (!out_device_id) return -1;

    for (int i = 0; i < MAX_ACCELERATORS; i++) {
        if (!accelerator_registry[i].active) {
            accelerator_registry[i].device_id = next_accel_device_id++;
            accelerator_registry[i].type = type;
            accelerator_registry[i].allocated_bytes = 0;
            accelerator_registry[i].active_jobs_count = 0;
            accelerator_registry[i].active = 1;

            *out_device_id = accelerator_registry[i].device_id;
            return 0;
        }
    }
    return -2; // Accelerator registry full
}

int accelerator_map_zero_copy_buffer(uint32_t device_id, const uint8_t *src_data, size_t size, uint32_t *out_offset) {
    if (!src_data || size == 0 || !out_offset) return -1;

    for (int i = 0; i < MAX_ACCELERATORS; i++) {
        if (accelerator_registry[i].active && accelerator_registry[i].device_id == device_id) {
            accelerator_device_t *dev = &accelerator_registry[i];

            if (dev->allocated_bytes + size > ACCEL_MEM_POOL_SIZE) {
                return -3; // Out of zero-copy shared memory space
            }

            size_t offset = dev->allocated_bytes;
            for (size_t j = 0; j < size; j++) {
                dev->shared_memory_pool[offset + j] = src_data[j];
            }

            dev->allocated_bytes += size;
            *out_offset = (uint32_t)offset;
            dev->active_jobs_count++;
            return 0;
        }
    }
    return -4; // Accelerator device ID not found
}
