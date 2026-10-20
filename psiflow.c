#include "psiflow.h"
#include "vtx_source.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void psiflow_init(void) {
    kernel_print("[PsiFlow] Initializing Tier 5 Surface Workflows & Vector Piping engine (.psi)...\n");
}

int psiflow_execute_pipeline(const char* psi_stream) {
    kernel_print("[PsiFlow] Executing vector pipe stream -> ");
    kernel_print(psi_stream);
    kernel_print("\n");

    if (psi_stream[0] == 'P' && psi_stream[1] == 'I' && psi_stream[2] == 'P' && psi_stream[3] == 'E') {
        kernel_print("[PsiFlow] Tunneling vector state across Aura Link interface...\n");
        const char *vtx_target = "STRUCT TensorPipeline { MAP: TENSOR_MAP(31) }";
        return vtx_compile_and_execute(vtx_target);
    }

    kernel_print("[PsiFlow] Error: Invalid PSI vector piping syntax.\n");
    return 0;
}
