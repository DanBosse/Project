#include "tier_scheduler.h"
#include "simd.h"

// Definition of the global tiers array referenced in your directory structure
execution_tier_t tiers[MAX_TIERS];
int current_tier_index = -1;
static int total_registered_tiers = 0;

void tier_scheduler_init(void) {
    for (int i = 0; i < MAX_TIERS; i++) {
        tiers[i].tier_id = 0;
        tiers[i].status_flag = TIER_STATUS_IDLE;
        tiers[i].engine_group = 0;
        tiers[i].state_vector = NULL;
        tiers[i].simd_fpu_context = NULL;
        tiers[i].execution_ticks = 0;
    }
    current_tier_index = -1;
    total_registered_tiers = 0;
}

int tier_register(uint64_t tier_id, uint32_t engine_group, void* state_vector) {
    if (total_registered_tiers >= MAX_TIERS) {
        return -1; // Out of tier slots
    }

    int idx = total_registered_tiers++;
    tiers[idx].tier_id = tier_id;
    tiers[idx].status_flag = TIER_STATUS_READY;
    tiers[idx].engine_group = engine_group;
    tiers[idx].state_vector = state_vector;
    
    // Allocate the dedicated SIMD/FPU context frame for this tier
    tiers[idx].simd_fpu_context = simd_allocate_context();
    tiers[idx].execution_ticks = 0;

    // If this is the first tier, mark it active
    if (current_tier_index == -1) {
        current_tier_index = idx;
        tiers[idx].status_flag = TIER_STATUS_RUNNING;
    }

    return idx;
}

void tier_switch(int next_index) {
    if (next_index < 0 || next_index >= total_registered_tiers) {
        return;
    }
    if (next_index == current_tier_index) {
        return;
    }

    int prev_index = current_tier_index;

    // 1. Save outgoing tier's SIMD/FPU context if valid
    if (prev_index != -1 && tiers[prev_index].simd_fpu_context) {
        simd_save(tiers[prev_index].simd_fpu_context);
        if (tiers[prev_index].status_flag == TIER_STATUS_RUNNING) {
            tiers[prev_index].status_flag = TIER_STATUS_READY;
        }
    }

    // 2. Update current index
    current_tier_index = next_index;
    tiers[current_tier_index].status_flag = TIER_STATUS_RUNNING;
    tiers[current_tier_index].execution_ticks++;

    // 3. Restore incoming tier's SIMD/FPU context if valid
    if (tiers[current_tier_index].simd_fpu_context) {
        simd_restore(tiers[current_tier_index].simd_fpu_context);
    }
}

// Simple round-robin tier scheduler hookable to your timer interrupt
void tier_schedule_tick(void) {
    if (total_registered_tiers <= 1) {
        return;
    }

    int next_index = (current_tier_index + 1) % total_registered_tiers;
    
    // Skip idle or suspended tiers
    int checked = 0;
    while (tiers[next_index].status_flag == TIER_STATUS_IDLE || 
           tiers[next_index].status_flag == TIER_STATUS_SUSPENDED) {
        next_index = (next_index + 1) % total_registered_tiers;
        checked++;
        if (checked >= total_registered_tiers) {
            return; // No runnable tiers found
        }
    }

    tier_switch(next_index);
}
