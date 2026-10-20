#include "studiolens.h"
#include "psiflow.h"

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

void studiolens_init(void) {
    kernel_print("[StudioLens] Initializing Tier 6 Studio Lens Environments & Spatial Workspace...\n");
}

int studiolens_render_workspace(const char* lens_name) {
    kernel_print("[StudioLens] Rendering active studio lens workspace -> ");
    kernel_print(lens_name);
    kernel_print("\n");

    kernel_print("[StudioLens] Projecting spatial lens view into PsiFlow vector pipe...\n");
    const char *psi_pipeline = "PIPE stream_vector TO TensorPipeline";
    return psiflow_execute_pipeline(psi_pipeline);
}
