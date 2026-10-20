#include "phasecore_interpreter.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void phasecore_interpreter_init(void) {
    kernel_print("[PhaseCoreInterpreter] Initializing deterministic execution engine...\n");
}

PhaseCoreOpcode phasecore_resolve_opcode(const char* name) {
    if (name[0] == 'T' && name[1] == 'O' && name[2] == 'R') return TORUS_LOOP;
    if (name[0] == 'P' && name[1] == 'H' && name[2] == 'A') return PHASE_SYNC;
    if (name[0] == 'M' && name[1] == 'E' && name[2] == 'M') return MEM_SPLICE;
    if (name[0] == 'O' && name[1] == 'P' && name[2] == 'T') return OPTIC_PULSE;
    if (name[0] == 'F' && name[1] == 'L' && name[2] == 'U') return FLUX_READ;
    if (name[0] == 'E' && name[1] == 'N' && name[2] == 'T') return ENTROPY_CLR;
    if (name[0] == 'T' && name[1] == 'E' && name[2] == 'N') return TENSOR_MAP;
    if (name[0] == 'W' && name[1] == 'A' && name[2] == 'V') return WAVE_INTERF;
    if (name[0] == 'M' && name[1] == 'A' && name[2] == 'S') return MASS_FILTER;
    if (name[0] == 'S' && name[1] == 'T' && name[2] == 'A' && name[3] == 'C') return STACK_FOLD;
    if (name[0] == 'P' && name[1] == 'I' && name[2] == 'P') return PIPE_STREAM;
    if (name[0] == 'S' && name[1] == 'T' && name[2] == 'A' && name[3] == 'T') return STATE_DECOMP;
    if (name[0] == 'E' && name[1] == 'X' && name[2] == 'E') return EXEC_HALT;
    if (name[0] == 'C' && name[1] == 'L' && name[2] == 'O') return CLOCK_PULSE;
    if (name[0] == 'V' && name[1] == 'E' && name[2] == 'C') return VECT_SHIFT;
    if (name[0] == 'C' && name[1] == 'O' && name[2] == 'R') return CORE_JUMP;
    
    return (PhaseCoreOpcode)0;
}

int phasecore_interpret_envelope(const char* xml_stream) {
    AsciiXmlElement elem;
    size_t err_off = 0;

    if (!ascii_xml_parse_node_safe(xml_stream, &elem, &err_off)) {
        kernel_print("[PhaseCoreInterpreter] Failed to parse opcode envelope.\n");
        return 0;
    }

    kernel_print("[PhaseCoreInterpreter] Interpreting tag: ");
    kernel_print(elem.tag);
    kernel_print("\n");

    uint64_t arg1 = 0;
    uint64_t arg2 = 0;

    for (int i = 0; i < elem.attr_count; i++) {
        if (elem.attrs[i].key[0] == 'a' && elem.attrs[i].key[4] == '1') {
            const char *p = elem.attrs[i].val;
            while (*p >= '0' && *p <= '9') {
                arg1 = arg1 * 10 + (*p - '0');
                p++;
            }
        } else if (elem.attrs[i].key[0] == 'a' && elem.attrs[i].key[4] == '2') {
            const char *p = elem.attrs[i].val;
            while (*p >= '0' && *p <= '9') {
                arg2 = arg2 * 10 + (*p - '0');
                p++;
            }
        }
    }

    PhaseCoreOpcode op = phasecore_resolve_opcode(elem.value);
    if (op != 0) {
        kernel_print("[PhaseCoreInterpreter] Executing resolved instruction -> ");
        kernel_print(elem.value);
        kernel_print("\n");
        phasecore_dispatch(op, arg1, arg2);
        return 1;
    } else {
        kernel_print("[PhaseCoreInterpreter] Error: Unrecognized opcode mnemonic in value.\n");
        return 0;
    }
}
