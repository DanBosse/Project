#include <stdint.h>
#include <stdbool.h>

void serial_print(const char *str);

typedef struct {
    uint32_t physical_refresh_hz;
    uint32_t render_fps;
    uint32_t vsync_counter;
    bool cadence_locked;
} AuraMotionEngine;

void auramotion_init_adaptive(AuraMotionEngine *motion, uint32_t detected_refresh_hz) {
    motion->physical_refresh_hz = detected_refresh_hz;

    // Dynamically adapt render ratio based on environment capabilities
    if (detected_refresh_hz >= 120) {
        motion->render_fps = 60; // Optimal 2:1 professional preview cadence
        motion->cadence_locked = true;
        serial_print("[AuraMotion] Adaptive Environment: High-refresh display detected (>=120Hz). Locking 2:1 cadence (120Hz / 60FPS).\n");
    } else {
        motion->render_fps = detected_refresh_hz; // 1:1 fallback for standard displays
        motion->cadence_locked = true;
        serial_print("[AuraMotion] Adaptive Environment: Standard display detected. Locking 1:1 cadence.\n");
    }
    motion->vsync_counter = 0;
}

bool auramotion_tick(AuraMotionEngine *motion) {
    motion->vsync_counter++;
    uint32_t ratio = motion->physical_refresh_hz / (motion->render_fps ? motion->render_fps : 1);
    return (motion->vsync_counter % (ratio ? ratio : 1)) == 0;
}
