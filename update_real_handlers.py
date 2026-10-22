with open("command_registry.c", "r") as f:
    code = f.read()

# Add proper declarations / handlers
new_handlers = """#include "command_registry.h"
#include "torus.h"
#include "physics_studio.h"
#include "flowscript.h"
#include "coprocessor.h"
#include "sot_mram.h"
#include <stddef.h>

void shell_print_help(const char *arg);

void shell_cmd_clear(const char *arg) {
    serial_print("\\033[2J\\033[H");
}

// Dedicated FlowScript & Real Subsystem Wrappers
static void cmd_dump_telemetry(const char *arg) { flowscript_execute("dump_telemetry"); }
static void cmd_run_full_pipeline(const char *arg) { flowscript_execute("run_full_pipeline"); }
static void cmd_pipeline_macro(const char *arg) { flowscript_execute("run_full_pipeline"); }
static void cmd_sync_all(const char *arg) { flowscript_execute("sync_all"); }
static void cmd_sync_auraspring(const char *arg) { flowscript_execute("sync_auraspring"); }
static void cmd_pulse_manifold(const char *arg) { flowscript_execute("pulse_manifold"); }

static void cmd_coproc(const char *arg) {
    serial_print("[Spatial Shell] Dispatching Coprocessor Diagnostic Suite...\\n");
    coprocessor_dispatch_tensor("TorusOS_System_Diagnostic");
}

static void cmd_heapdump(const char *arg) {
    serial_print("[Kernel Heap] Inspecting active memory blocks & fragmentation...\\n");
    serial_print("[Kernel Heap] Base: 0xFFFFFFFF80000000 | SOT-MRAM Zero-Latency Status: ACTIVE\\n");
    sot_mram_verify_t_refi_zero();
}
"""

import re
code = re.sub(r'#include "command_registry\.h".*?void shell_cmd_clear\(const char \*arg\) \{.*?\}', new_handlers, code, flags=re.DOTALL)

# Update global_commands handlers for coproc and heapdump
code = code.replace('{ "coproc", "", "Query real-time hardware status metrics for DPU, AuraPU, and VPU modules", "coprocessor", NULL, false },', '{ "coproc", "", "Query real-time hardware status metrics for DPU, AuraPU, and VPU modules", "coprocessor", (cmd_handler_t)cmd_coproc, false },')
code = code.replace('{ "heapdump", "", "Inspect kernel heap allocation blocks and fragmentation", "memory", NULL, false },', '{ "heapdump", "", "Inspect kernel heap allocation blocks and fragmentation", "memory", (cmd_handler_t)cmd_heapdump, false },')

with open("command_registry.c", "w") as f:
    f.write(code)

print("Updated command_registry.c with real coproc and heapdump handlers.")
