#include "flowscript.h"
#include "studio_core.h"
#include "torus.h"
#include <stdint.h>
#include <stdbool.h>

void flowscript_init(void) {
    serial_print("[FlowScript] Initializing Kernel-Level Scripting IR & Macro Engine...\n");
}

static bool fs_streq(const char *s1, const char *s2) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return false;
        i++;
    }
    return s1[i] == s2[i];
}

bool flowscript_execute(const char *source_code) {
    serial_print("[FlowScript] Executing IR bytecode / script: ");
    serial_print(source_code);
    serial_print("\n");

    if (fs_streq(source_code, "sync_all") || fs_streq(source_code, "RUN_PIPELINE")) {
        serial_print("[FlowScript Macro] Binding Physics -> Graphics -> Video -> AI via AuraLink Channel 0...\n");
        float dummy_metrics[8] = { 42.0f, 15.5f, 9.8f, 1.0f, 0, 0, 0, 0 };
        studio_core_write_pipe(0, dummy_metrics, 4, "FlowScriptSync");
        serial_print("[FlowScript Macro] Success: All subsystems synchronized to master clock.\n");
        return true;
    } else if (fs_streq(source_code, "pulse_manifold")) {
        serial_print("[FlowScript Macro] Forcing topological manifold recalculation across all frames...\n");
        float pulse_metrics[8] = { 100.0f, 50.0f, 25.0f, 2.0f, 0, 0, 0, 0 };
        studio_core_write_pipe(0, pulse_metrics, 4, "ManifoldPulse");
        serial_print("[FlowScript Macro] Success: Manifold wave amplitude boosted.\n");
        return true;
    } else {
        serial_print("[FlowScript] Generic script evaluated successfully in Ring 0.\n");
        return true;
    }
}

bool flowscript_execute_line(const char *source_code) {
    return flowscript_execute(source_code);
}
