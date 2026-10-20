#ifndef PHASECORE_INTERPRETER_H
#define PHASECORE_INTERPRETER_H

#include "phasecore.h"
#include "ascii_xml.h"
#include <stdint.h>
#include <stddef.h>

void phasecore_interpreter_init(void);
PhaseCoreOpcode phasecore_resolve_opcode(const char* name);
int phasecore_interpret_envelope(const char* xml_stream);

#endif
