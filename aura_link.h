#ifndef AURA_LINK_H
#define AURA_LINK_H

#include <stdint.h>
#include <stdbool.h>

void aura_link_init(void);
uint64_t aura_link_transmit_telemetry(uint64_t state_packet);
bool aura_link_verify_backplane(uint64_t link_handle);

#endif
