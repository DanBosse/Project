#include <stdint.h>
#include <stddef.h>

#define MAX_SILICON_DEVICES 16

typedef enum {
    SILICON_TYPE_CPU = 1,
    SILICON_TYPE_GPU,
    SILICON_TYPE_NPU,
    SILICON_TYPE_DPU
} silicon_type_t;

typedef struct {
    uint32_t device_id;
    silicon_type_t type;
    uint32_t utilization_percent;
    uint32_t temperature_celsius;
    uint32_t memory_pressure_percent;
    uint32_t active_inference_jobs;
    uint8_t online;
} silicon_device_node_t;

static silicon_device_node_t silicon_pool[MAX_SILICON_DEVICES];
static uint32_t next_silicon_id = 8000;

void ai_load_balancer_init(void) {
    for (int i = 0; i < MAX_SILICON_DEVICES; i++) {
        silicon_pool[i].online = 0;
        silicon_pool[i].active_inference_jobs = 0;
    }
}

int ai_load_balancer_register_device(silicon_type_t type, uint32_t *out_device_id) {
    if (!out_device_id) return -1;

    for (int i = 0; i < MAX_SILICON_DEVICES; i++) {
        if (!silicon_pool[i].online) {
            silicon_device_node_t *dev = &silicon_pool[i];
            dev->device_id = next_silicon_id++;
            dev->type = type;
            dev->utilization_percent = 10; // Nominal start load
            dev->temperature_celsius = 45;
            dev->memory_pressure_percent = 15;
            dev->active_inference_jobs = 0;
            dev->online = 1;

            *out_device_id = dev->device_id;
            return 0;
        }
    }
    return -2; // Silicon device registry full
}

int ai_load_balancer_route_inference(silicon_type_t preferred_type, uint32_t workload_weight, uint32_t *out_selected_device) {
    if (!out_selected_device) return -1;

    int best_device_idx = -1;
    uint32_t lowest_score = 0xFFFFFFFF;

    // Intelligent heuristic: Score = utilization + memory_pressure + temperature penalty
    for (int i = 0; i < MAX_SILICON_DEVICES; i++) {
        if (silicon_pool[i].online) {
            silicon_device_node_t *dev = &silicon_pool[i];

            // Match preferred type if available, otherwise consider all online accelerators
            uint32_t type_match_bonus = (dev->type == preferred_type) ? 0 : 50;
            uint32_t score = dev->utilization_percent + dev->memory_pressure_percent + (dev->temperature_celsius / 2) + type_match_bonus;

            if (score < lowest_score && dev->temperature_celsius < 85) { // Thermal cutoff safety
                lowest_score = score;
                best_device_idx = i;
            }
        }
    }

    if (best_device_idx == -1) {
        return -2; // No suitable silicon devices available or thermal limits exceeded
    }

    silicon_device_node_t *selected = &silicon_pool[best_device_idx];
    selected->utilization_percent += workload_weight;
    if (selected->utilization_percent > 100) selected->utilization_percent = 100;
    selected->active_inference_jobs++;

    *out_selected_device = selected->device_id;
    return 0;
}
