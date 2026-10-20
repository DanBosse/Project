#ifndef INVARIANTS_H
#define INVARIANTS_H

#include <stdint.h>

#define MAX_INVARIANTS 16

// Invariant violation severity levels
#define SEV_WARNING 1
#define SEV_CRITICAL 2
#define SEV_FATAL    3

typedef struct {
    uint32_t invariant_id;
    uint32_t severity;
    const char* description;
    uint64_t violation_count;
    uint32_t active_flag;
} system_invariant_t;

// Initialize core system invariants
void invariants_init(void);

// Register a custom invariant rule
int invariant_register(uint32_t id, uint32_t severity, const char* desc);

// Evaluate all active invariants against current tier state vectors
int invariants_evaluate_all(void);

// Retrieve total recorded invariant violations across the system
uint64_t invariants_get_total_violations(void);

#endif // INVARIANTS_H
