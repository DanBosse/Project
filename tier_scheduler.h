#ifndef TIER_SCHEDULER_H
#define TIER_SCHEDULER_H

#include <stdint.h>
#include <stddef.h>

#define MAX_TIERS 32

// Status flags
#define TIER_STATUS_IDLE      0x00
#define TIER_STATUS_READY     0x01
#define TIER_STATUS_RUNNING   0x02
#define TIER_STATUS_BLOCKED   0x03
#define TIER_STATUS_SUSPENDED 0x04

typedef struct {
    uint64_t tier_id;
    uint32_t status_flag;
    uint32_t engine_group;
    void* state_vector;       // Multi-dimensional state vector mapping
    void* simd_fpu_context;   // Aligned 64-byte SIMD save area (from Module 1)
    uint64_t execution_ticks; // Performance / operational metric tracking
} execution_tier_t;

// Global tiers array matching your project layout
extern execution_tier_t tiers[MAX_TIERS];
extern int current_tier_index;

// Initialize the tier scheduler subsystem
void tier_scheduler_init(void);

// Register or spawn a new tier
int tier_register(uint64_t tier_id, uint32_t engine_group, void* state_vector);

// Core scheduling tick function (called by timer interrupt)
void tier_schedule_tick(void);

// Manual context switch between two tiers
void tier_switch(int next_index);

#endif // TIER_SCHEDULER_H
