registry_content = """#include "command_registry.h"
#include "torus.h"
#include "physics_studio.h"
#include "flowscript.h"
#include <stddef.h>

void shell_print_help(const char *arg);

void shell_cmd_clear(const char *arg) {
    serial_print("\\033[2J\\033[H");
}

static command_t global_commands[] = {
    // System & General
    { "help", "[category]", "Display this help menu or filter by category", "system", (cmd_handler_t)shell_print_help, false },
    { "status", "", "Show subsystem & tier metrics", "system", NULL, false },
    { "clear", "", "Clear terminal output", "system", (cmd_handler_t)shell_cmd_clear, false },
    { "sandbox", "", "Launch Ring 3/0 secure isolation sandbox", "system", NULL, false },

    // Telemetry & Monitoring
    { "dashboard", "", "Render StudioLens live telemetry dashboard", "telemetry", NULL, false },
    { "telemetry_dump", "", "Inspect Ring 0 zero-copy AuraLink telemetry channels", "telemetry", NULL, false },
    { "dump_telemetry", "", "Dump FlowScript runtime telemetry metrics", "telemetry", (cmd_handler_t)flowscript_execute, false },
    { "auralink", "", "Verify backplane telemetry link", "telemetry", NULL, false },

    // Pipelines & Physics Studio
    { "pipeline_full", "", "Run multi-step AuraSpring -> Graphics -> Audio -> Video pipeline", "pipeline", (cmd_handler_t)physics_studio_full_pipeline, false },
    { "run_full_pipeline", "", "Execute full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)flowscript_execute, false },
    { "auraspring", "", "Launch AuraSpring physics simulation workspace", "pipeline", NULL, false },
    { "physicslab", "", "Launch AuraSpring physics simulation workspace", "pipeline", NULL, false },
    { "physicstest", "", "Run AuraSpring physics simulation benchmark", "pipeline", NULL, false },
    { "sync_all", "", "Synchronize all active pipeline manifolds", "pipeline", (cmd_handler_t)flowscript_execute, false },
    { "sync_auraspring", "", "Synchronize AuraSpring state channel", "pipeline", (cmd_handler_t)flowscript_execute, false },
    { "pulse_manifold", "", "Pulse Frequency Manifold synchronization wave", "pipeline", (cmd_handler_t)flowscript_execute, false },

    // Coprocessors & AI
    { "aurapu", "", "Execute live AuraPU NPU tensor inference & benchmark", "coprocessor", NULL, false },
    { "dpu", "", "Test AuraDPU zero-copy fabric & stream offload", "coprocessor", NULL, false },
    { "vpu", "", "Test AuraVPU GPU coprocessor ray tracing & render offload", "coprocessor", NULL, false },
    { "aura", "", "Launch Aura OS Artificial Intelligence Core", "ai", NULL, false },
    { "auratest", "", "Run Aura AI core & transformer benchmark", "ai", NULL, false },

    // Studios & Spatial Compositor
    { "studiolens", "[grid|stacked|spawn|workspace]", "Render Tier 6 spatial workspace", "studios", NULL, false },
    { "videostudio", "", "Launch StudioLens Video NLE Suite", "studios", NULL, false },
    { "audiostudio", "", "Launch StudioLens Audio DAW & Mixer", "studios", NULL, false },
    { "graphicsstudio", "", "Launch StudioLens Graphics & Design Canvas", "studios", NULL, false },
    { "office", "", "Launch Sovereign Office document productivity suite", "studios", NULL, false },
    { "cad", "", "Launch AuraCad Parametric CAD & Physics Suite", "studios", NULL, false },

    // Memory & Storage
    { "mram", "", "Test SOT-MRAM zero-latency write/read", "memory", NULL, false },
    { "twinalloc", "[size]", "Allocate memory block via twin primes", "memory", NULL, false },
    { "benchmark", "", "[Phase 1] Run MRAM & Twin-Prime saturation suite", "memory", NULL, false },

    // Hidden / Advanced Opcodes
    { "phasecore", "", "[Hidden] Execute raw Tier 1 opcode dispatch", "hidden", NULL, true },
    { "omni", "", "[Hidden] Unpack Sovereign Omnifile into AuraScript", "hidden", NULL, true },
    { "vtx", "", "[Hidden] Compile structural tensors to FlowScript IR", "hidden", NULL, true },
};

void shell_print_help(const char *category) {
    serial_print("[TorusOS Spatial Shell] === Command Registry & Help ===\\n");
    if (category && category[0] != '\\0') {
        serial_print("Filtered by category: ");
        serial_print(category);
        serial_print("\\n---------------------------------------------------------\\n");
    }

    for (size_t i = 0; i < sizeof(global_commands) / sizeof(command_t); i++) {
        if (global_commands[i].hidden) continue;

        if (category && category[0] != '\\0') {
            const char *c = global_commands[i].category;
            bool match = true;
            for (int j = 0; category[j] != '\\0' && c[j] != '\\0'; j++) {
                if (category[j] != c[j]) { match = false; break; }
            }
            if (!match) continue;
        }

        serial_print("  ");
        serial_print(global_commands[i].name);
        serial_print(" ");
        serial_print(global_commands[i].args);
        serial_print(" - ");
        serial_print(global_commands[i].description);
        serial_print("\\n");
    }
    serial_print("=========================================================\\n");
    serial_print("Tip: Use 'help [category]' (e.g. 'help telemetry' or 'help pipeline')\\n");
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
                global_commands[i].handler(cmd);
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

print("Expanded command_registry.c with all FlowScript and pipeline commands.")
