#include "invariants.h"
#include "tier_scheduler.h"
#include <stdint.h>
#include <stddef.h>

#define MAX_INVARIANTS 16

static system_invariant_t registered_invariants[MAX_INVARIANTS];
static int invariant_count = 0;
static uint64_t total_violations = 0;

void invariants_init(void) {
    invariant_count = 0;
    total_violations = 0;
    for (int i = 0; i < MAX_INVARIANTS; i++) {
        registered_invariants[i].invariant_id = 0;
        registered_invariants[i].severity = 0;
        registered_invariants[i].description = NULL;
        registered_invariants[i].violation_count = 0;
        registered_invariants[i].active_flag = 0;
    }
}

int invariant_register(uint32_t id, uint32_t severity, const char* desc) {
    if (invariant_count >= MAX_INVARIANTS) {
        return -1;
    }
    int idx = invariant_count++;
    registered_invariants[idx].invariant_id = id;
    registered_invariants[idx].severity = severity;
    registered_invariants[idx].description = desc;
    registered_invariants[idx].violation_count = 0;
    registered_invariants[idx].active_flag = 1;
    return idx;
}

int invariants_evaluate_all(void) {
    int violations_found = 0;
    for (int i = 0; i < MAX_TIERS; i++) {
        if (tiers[i].status_flag == TIER_STATUS_RUNNING && tiers[i].state_vector == NULL) {
            violations_found++;
            total_violations++;
            if (invariant_count > 0) {
                registered_invariants[0].violation_count++;
            }
        }
    }
    return violations_found;
}

uint64_t invariants_get_total_violations(void) {
    return total_violations;
}
