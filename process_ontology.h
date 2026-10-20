#ifndef PROCESS_ONTOLOGY_H
#define PROCESS_ONTOLOGY_H

#include <stdint.h>
#include <stdbool.h>

void process_ontology_init(void);
uint64_t process_collapse_phase(uint64_t current_process_handle);
bool process_verify_ontology(uint64_t process_handle);

#endif
