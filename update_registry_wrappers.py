registry_content = """#include "command_registry.h"
#include "torus.h"
#include "physics_studio.h"
#include "flowscript.h"
#include <stddef.h>

void shell_print_help(const char *arg);

void shell_cmd_clear(const char *arg) {
    serial_print("\\033[2J\\033[H");
}

// Dedicated FlowScript wrappers
static void cmd_dump_telemetry(const char *arg) { flowscript_execute("dump_telemetry"); }
static void cmd_run_full_pipeline(const char *arg) { flowscript_execute("run_full_pipeline"); }
static void cmd_pipeline_macro(const char *arg) { flowscript_execute("run_full_pipeline"); }
static void cmd_sync_all(const char *arg) { flowscript_execute("sync_all"); }
static void cmd_sync_auraspring(const char *arg) { flowscript_execute("sync_auraspring"); }
static void cmd_pulse_manifold(const char *arg) { flowscript_execute("pulse_manifold"); }

static command_t global_commands[] = {
    // System & General
    { "help", "[category]", "Display this help menu or filter by category", "system", (cmd_handler_t)shell_print_help, false },
    { "status", "", "Show subsystem & tier metrics", "system", NULL, false },
    { "clear", "", "Clear terminal output", "system", (cmd_handler_t)shell_cmd_clear, false },
    { "sandbox", "", "Launch Ring 3/0 secure isolation sandbox", "system", NULL, false },
    { "reboot", "", "Reboot TorusOS kernel via ACPI", "system", NULL, false },
    { "shutdown", "", "Halt system and power down", "system", NULL, false },
    { "systems", "", "List and manage initialized Tier 1-6 core system profiles", "system", NULL, false },
    { "subsystems", "", "Query real-time modular convergence subsystem links", "system", NULL, false },

    // Security & Infrastructure
    { "security", "", "Check attestation profiles and system vulnerability logs", "security", NULL, false },
    { "firewall", "", "Query real-time rule sets and drop states for network boundaries", "security", NULL, false },
    { "server", "", "Monitor active background network servers and IPC socket states", "security", NULL, false },
    { "scheduler", "", "Query active process schedule structures and timer interrupt rates", "security", NULL, false },
    { "yield", "", "Trigger a manual task rotation via the cooperative context hook", "security", NULL, false },
    { "mathhook", "", "Inspect initialized algebraic evaluation layers at kernel boot", "security", NULL, false },

    // Telemetry & Monitoring
    { "dashboard", "", "Render StudioLens live telemetry dashboard", "telemetry", NULL, false },
    { "telemetry_dump", "", "Inspect Ring 0 zero-copy AuraLink telemetry channels", "telemetry", NULL, false },
    { "dump_telemetry", "", "Dump FlowScript runtime telemetry metrics", "telemetry", (cmd_handler_t)cmd_dump_telemetry, false },
    { "auralink", "", "Inspect Ring 0 zero-copy optical backplane telemetry channels", "telemetry", NULL, false },
    { "metrics", "", "Display real-time CPU, MRAM, and NPU load counters", "telemetry", NULL, false },

    // Pipelines, FlowScript & Physics Studio
    { "pipeline_full", "", "Run multi-step AuraSpring -> Graphics -> Audio -> Video pipeline", "pipeline", (cmd_handler_t)physics_studio_full_pipeline, false },
    { "pipeline", "", "Run full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)cmd_pipeline_macro, false },
    { "run_full_pipeline", "", "Execute full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)cmd_run_full_pipeline, false },
    { "auraspring", "", "Launch AuraSpring physics simulation workspace", "pipeline", NULL, false },
    { "physicslab", "", "Load physics-inspired parametric engineering simulation canvas", "pipeline", NULL, false },
    { "physics", "", "Alias for physicslab workspace suite", "pipeline", NULL, false },
    { "pyhsics", "", "Alias for physicslab workspace suite", "pipeline", NULL, false },
    { "physicstest", "", "Run AuraSpring physics simulation benchmark", "pipeline", NULL, false },
    { "sync_all", "", "Synchronize all active pipeline manifolds", "pipeline", (cmd_handler_t)cmd_sync_all, false },
    { "sync_auraspring", "", "Synchronize AuraSpring state channel", "pipeline", (cmd_handler_t)cmd_sync_auraspring, false },
    { "pulse_manifold", "", "Pulse Frequency Manifold synchronization wave", "pipeline", (cmd_handler_t)cmd_pulse_manifold, false },

    // Coprocessors & AI
    { "coproc", "", "Query real-time hardware status metrics for DPU, AuraPU, and VPU modules", "coprocessor", NULL, false },
    { "aurapu", "", "Query Sovereign NPU Core metrics", "coprocessor", NULL, false },
    { "dpu", "", "Query Data Processing Unit metrics", "coprocessor", NULL, false },
    { "vpu", "", "Query Vision Processing Unit metrics", "coprocessor", NULL, false },
    { "aura", "", "Query AuraScript parsers and AuraLink optical backplane states", "ai", NULL, false },
    { "auratest", "", "Verify AuraLink cross-subsystem pipeline distribution", "ai", NULL, false },

    // Studios & Spatial Compositor
    { "studio", "", "Launch StudioLens Spatial Environment", "studios", NULL, false },
    { "studios", "", "Enumerate loaded StudioLens spatial creative suites", "studios", NULL, false },
    { "studiolens", "[grid|stacked|spawn|workspace]", "Render Tier 6 spatial workspace", "studios", NULL, false },
    { "videostudio", "", "Open wide-gamut cinematic video editing suite", "studios", NULL, false },
    { "audiostudio", "", "Launch spatial low-latency audio production stack", "studios", NULL, false },
    { "graphicsstudio", "", "Initialize vector workspace & motion engines", "studios", NULL, false },
    { "office", "", "Launch Sovereign Office document productivity suite", "studios", NULL, false },
    { "cad", "", "Launch AuraCad Parametric CAD & Physics Suite", "studios", NULL, false },
    { "labs", "", "List experimental secure sandboxed kernel testing environments", "studios", NULL, false },

    // Memory & Storage
    { "mram", "", "Test SOT-MRAM zero-latency write/read", "memory", NULL, false },
    { "twinalloc", "[size]", "Allocate memory block via twin primes", "memory", NULL, false },
    { "benchmark", "", "[Phase 1] Run MRAM & Twin-Prime saturation suite", "memory", NULL, false },
    { "heapdump", "", "Inspect kernel heap allocation blocks and fragmentation", "memory", NULL, false },

    // Hidden / Advanced Opcodes
    { "phasecore", "", "[Hidden] Execute raw Tier 1 opcode dispatch", "hidden", NULL, true },
    { "omni", "", "[Hidden] Unpack Sovereign Omnifile into AuraScript", "hidden", NULL, true },
    { "vtx", "", "[Hidden] Compile structural tensors to FlowScript IR", "hidden", NULL, true },
    { "debugdump", "", "[Hidden] Dump raw CPU registers and stack trace", "hidden", NULL, true },
};

void shell_print_help(const char *arg) {
    char cat_buf[32];
    int idx = 0;
    if (arg) {
        while (*arg == ' ' || *arg == '\\"' || *arg == '\\'') arg++;
        while (arg[idx] != '\\0' && idx < 31) {
            if (arg[idx] == '\\"' || arg[idx] == '\\'' || arg[idx] == '\\n' || arg[idx] == '\\r') break;
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
"""

with open("command_registry.c", "w") as f:
    f.write(registry_content)

print("Updated command_registry.c with robust FlowScript wrappers.")
