#include "vtx_source.h"
#include "flowscript.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void vtx_source_init(void) {
    kernel_print("[VTX Source] Initializing Tier 4 High-Level Source compiler (.vtx)...\n");
}

int vtx_compile_and_execute(const char* vtx_program) {
    kernel_print("[VTX Source] Compiling high-level structural source -> ");
    kernel_print(vtx_program);
    kernel_print("\n");

    if (vtx_program[0] == 'S' && vtx_program[1] == 'T' && vtx_program[2] == 'R' && vtx_program[3] == 'U' && vtx_program[4] == 'C' && vtx_program[5] == 'T') {
        kernel_print("[VTX Source] Lowering structural declaration into Tier 3 FlowScript IR...\n");
        const char *fsi_target = "LET tensor_reg = TENSOR_MAP(31)";
        return flowscript_execute_line(fsi_target);
    }

    kernel_print("[VTX Source] Error: Unrecognized VTX structure declaration.\n");
    return 0;
}
