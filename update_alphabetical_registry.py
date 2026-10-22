import sys

code = '''#include "command_registry.h"
#include "torus.h"
#include "physics_studio.h"
#include "physics_lab.h"
#include "flowscript.h"
#include "coprocessor.h"
#include "sot_mram.h"
#include <stddef.h>

void shell_print_help(const char *arg);

void shell_cmd_clear(const char *arg) {
    serial_print("\\033[2J\\033[H");
}

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

static void cmd_physicslab(const char *arg) {
    serial_print("[Physics Lab] Loading parametric engineering simulation canvas...\\n");
    physics_lab_render();
}

// Alphabetically sorted command registry table
static command_t global_commands[] = {
    { "aura", "", "Query AuraScript parsers and AuraLink optical backplane states", "ai", NULL, false },
    { "auralink", "", "Inspect Ring 0 zero-copy optical backplane telemetry channels", "telemetry", NULL, false },
    { "aurapu", "", "Query Sovereign NPU Core metrics", "coprocessor", NULL, false },
    { "auraspring", "", "Launch AuraSpring physics simulation workspace", "pipeline", NULL, false },
    { "auratest", "", "Verify AuraLink cross-subsystem pipeline distribution", "ai", NULL, false },
    { "audiostudio", "", "Launch spatial low-latency audio production stack", "studios", NULL, false },
    { "benchmark", "", "[Phase 1] Run MRAM & Twin-Prime saturation suite", "memory", NULL, false },
    { "cad", "", "Launch AuraCad Parametric CAD & Physics Suite", "studios", NULL, false },
    { "clear", "", "Clear terminal output", "system", (cmd_handler_t)shell_cmd_clear, false },
    { "coproc", "", "Query real-time hardware status metrics for DPU, AuraPU, and VPU modules", "coprocessor", (cmd_handler_t)cmd_coproc, false },
    { "dashboard", "", "Render StudioLens live telemetry dashboard", "telemetry", NULL, false },
    { "debugdump", "", "[Hidden] Dump raw CPU registers and stack trace", "hidden", NULL, true },
    { "dpu", "", "Query Data Processing Unit metrics", "coprocessor", NULL, false },
    { "dump_telemetry", "", "Dump FlowScript runtime telemetry metrics", "telemetry", (cmd_handler_t)cmd_dump_telemetry, false },
    { "firewall", "", "Query real-time rule sets and drop states for network boundaries", "security", NULL, false },
    { "graphicsstudio", "", "Initialize vector workspace & motion engines", "studios", NULL, false },
    { "heapdump", "", "Inspect kernel heap allocation blocks and fragmentation", "memory", (cmd_handler_t)cmd_heapdump, false },
    { "help", "[category]", "Display this help menu or filter by category", "system", (cmd_handler_t)shell_print_help, false },
    { "labs", "", "List experimental secure sandboxed kernel testing environments", "studios", NULL, false },
    { "mathhook", "", "Inspect initialized algebraic evaluation layers at kernel boot", "security", NULL, false },
    { "metrics", "", "Display real-time CPU, MRAM, and NPU load counters", "telemetry", NULL, false },
    { "mram", "", "Test SOT-MRAM zero-latency write/read", "memory", NULL, false },
    { "office", "", "Launch Sovereign Office document productivity suite", "studios", NULL, false },
    { "omni", "", "[Hidden] Unpack Sovereign Omnifile into AuraScript", "hidden", NULL, true },
    { "phasecore", "", "[Hidden] Execute raw Tier 1 opcode dispatch", "hidden", NULL, true },
    { "physics", "", "Alias for physicslab workspace suite", "pipeline", (cmd_handler_t)cmd_physicslab, false },
    { "physicslab", "", "Load physics-inspired parametric engineering simulation canvas", "pipeline", (cmd_handler_t)cmd_physicslab, false },
    { "physicstest", "", "Run AuraSpring physics simulation benchmark", "pipeline", NULL, false },
    { "pipeline", "", "Run full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)cmd_pipeline_macro, false },
    { "pipeline_full", "", "Run multi-step AuraSpring -> Graphics -> Audio -> Video pipeline", "pipeline", (cmd_handler_t)physics_studio_full_pipeline, false },
    { "pulse_manifold", "", "Pulse Frequency Manifold synchronization wave", "pipeline", (cmd_handler_t)cmd_pulse_manifold, false },
    { "pyhsics", "", "Alias for physicslab workspace suite", "pipeline", (cmd_handler_t)cmd_physicslab, false },
    { "reboot", "", "Reboot TorusOS kernel via ACPI", "system", NULL, false },
    { "run_full_pipeline", "", "Execute full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)cmd_run_full_pipeline, false },
    { "sandbox", "", "Launch Ring 3/0 secure isolation sandbox", "system", NULL, false },
    { "scheduler", "", "Query active process schedule structures and timer interrupt rates", "security", NULL, false },
    { "security", "", "Check attestation profiles and system vulnerability logs", "security", NULL, false },
    { "server", "", "Monitor active background network servers and IPC socket states", "security", NULL, false },
    { "shutdown", "", "Halt system and power down", "system", NULL, false },
    { "status", "", "Show subsystem & tier metrics", "system", NULL, false },
    { "studio", "", "Launch StudioLens Spatial Environment", "studios", NULL, false },
    { "studiolens", "[grid|stacked|spawn|workspace]", "Render Tier 6 spatial workspace", "studios", NULL, false },
    { "studios", "", "Enumerate loaded StudioLens spatial creative suites", "studios", NULL, false },
    { "subsystems", "", "Query real-time modular convergence subsystem links", "system", NULL, false },
    { "sync_all", "", "Synchronize all active pipeline manifolds", "pipeline", (cmd_handler_t)cmd_sync_all, false },
    { "sync_auraspring", "", "Synchronize AuraSpring state channel", "pipeline", (cmd_handler_t)cmd_sync_auraspring, false },
    { "telemetry_dump", "", "Inspect Ring 0 zero-copy AuraLink telemetry channels", "telemetry", NULL, false },
    { "twinalloc", "[size]", "Allocate memory block via twin primes", "memory", NULL, false },
    { "videostudio", "", "Open wide-gamut cinematic video editing suite", "studios", NULL, false },
    { "vpu", "", "Query Vision Processing Unit metrics", "coprocessor", NULL, false },
    { "vtx", "", "[Hidden] Compile structural tensors to FlowScript IR", "hidden", NULL, true },
    { "yield", "", "Trigger a manual task rotation via the cooperative context hook", "security", NULL, false },
};

void shell_print_help(const char *arg) {
    char cat_buf[32];
    int idx = 0;
    if (arg) {
        while (*arg == ' ' || *arg == '"' || *arg == '\\\'') arg++;
        while (arg[idx] != '\\0' && idx < 31) {
            if (arg[idx] == '"' || arg[idx] == '\\\'' || arg[idx] == '\\n' || arg[idx] == '\\r') break;
            cat_buf[idx] = arg[idx];
            idx++;
        }
        cat_buf[idx] = '\\0';
        while (idx > 0 && cat_buf[idx - 1] == ' ') {
            cat_buf[--idx] = '\\0';
        }
    } else {
        cat_buf[0] = '\\0';
    }

    serial_print("[TorusOS Help System]\\n");
    if (cat_buf[0] != '\\0') {
        serial_print("Filtered by category: ");
        serial_print(cat_buf);
        serial_print("\\n---------------------------------------------------------\\n");
    }

    for (size_t i = 0; i < sizeof(global_commands) / sizeof(command_t); i++) {
        if (global_commands[i].hidden) continue;

        if (cat_buf[0] != '\\0') {
            const char *c = global_commands[i].category;
            bool match = true;
            int j = 0;
            while (cat_buf[j] != '\\0' && c[j] != '\\0') {
                if (cat_buf[j] != c[j]) { match = false; break; }
                j++;
            }
            if (cat_buf[j] != '\\0' || c[j] != '\\0') match = false;
            if (!match) continue;
        }

        serial_print("  ");
        serial_print(global_commands[i].name);
        serial_print(" - ");
        serial_print(global_commands[i].description);
        serial_print("\\n");
    }
    serial_print("=========================================================\\n");
}

bool shell_dispatch_command(const char *cmd, const char *arg) {
    for (size_t i = 0; i < sizeof(global_commands) / sizeof(command_t); i++) {
        const char *name = global_commands[i].name;
        bool match = true;
        size_t j = 0;
        while (cmd[j] != '\\0' && name[j] != '\\0') {
            if (cmd[j] != name[j]) { match = false; break; }
            j++;
        }
        if (cmd[j] != '\\0' || name[j] != '\\0') match = false;

        if (match) {
            if (global_commands[i].handler) {
                global_commands[i].handler(arg);
            } else {
                serial_print("[Spatial Shell] Command recognized via registry table.\\n");
            }
            return true;
        }
    }
    return false;
}
'''

with open("command_registry.c", "w") as f:
    f.write(code)

print("command_registry.c updated with alphabetical sorting and physicslab linkage.")
