#include <stdint.h>
#include <stddef.h>

typedef enum {
    POWER_PROFILE_PERFORMANCE = 0,
    POWER_PROFILE_BALANCED,
    POWER_PROFILE_POWERSAVE,
    POWER_PROFILE_TURBO
} power_profile_t;

typedef struct {
    power_profile_t current_profile;
    uint32_t thermal_limit_celsius;
    uint32_t current_temp_celsius;
    uint8_t accelerator_gated;
    uint32_t voltage_mv;
} power_state_t;

static power_state_t system_power_state;

void power_governor_init(void) {
    system_power_state.current_profile = POWER_PROFILE_BALANCED;
    system_power_state.thermal_limit_celsius = 85;
    system_power_state.current_temp_celsius = 42;
    system_power_state.accelerator_gated = 0;
    system_power_state.voltage_mv = 1200; // Standard baseline voltage
}

int power_governor_set_profile(power_profile_t profile) {
    system_power_state.current_profile = profile;
    switch (profile) {
        case POWER_PROFILE_PERFORMANCE:
            system_power_state.voltage_mv = 1350;
            break;
        case POWER_PROFILE_BALANCED:
            system_power_state.voltage_mv = 1200;
            break;
        case POWER_PROFILE_POWERSAVE:
            system_power_state.voltage_mv = 1000;
            break;
        case POWER_PROFILE_TURBO:
            system_power_state.voltage_mv = 1420;
            break;
    }
    return 0;
}

int power_governor_update_temperature(uint32_t current_temp) {
    system_power_state.current_temp_celsius = current_temp;
    if (current_temp >= system_power_state.thermal_limit_celsius) {
        // Trigger thermal throttling / power gating safety fallback
        system_power_state.accelerator_gated = 1;
        return -1; // Thermal warning limit exceeded
    }
    system_power_state.accelerator_gated = 0;
    return 0;
}

const power_state_t* power_governor_get_state(void) {
    return &system_power_state;
}
