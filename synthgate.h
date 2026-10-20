#ifndef SYNTHGATE_H
#define SYNTHGATE_H

#include <stdint.h>
#include <stdbool.h>

void synthgate_init(void);
uint64_t synthgate_synthesize_master(void);
bool synthgate_verify_master_lock(uint64_t master_handle);

#endif
