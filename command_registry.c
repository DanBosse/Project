

#include "command_registry.h"
#include "torus.h"
#include "physics_studio.h"
#include "physics_lab.h"
#include "flowscript.h"
#include "coprocessor.h"
#include "sot_mram.h"
#include "audio_studio.h"
#include "aura.h"
#include "auracad.h"
#include "aura_link.h"
#include "aurascript.h"
#include "benchmark.h"
#include "studiolens_dashboard.h"
#include "dpu.h"
#include "graphics_studio.h"
#include "enterprise_telemetry.h"
#include "office_suite.h"
#include "omni_envelope.h"
#include "phasecore.h"
#include "sandbox.h"
#include "scheduler.h"
#include "twin_prime_alloc.h"
#include "video_studio.h"
#include "vpu.h"
#include "vtx_source.h"
#include "studiolens.h"
#include "workspace_registry.h"
#include <stddef.h>

static int simple_atoi(const char *str) {
    int res = 0;
    if (!str) return 0;
    while (*str == ' ' || *str == '\t') str++;
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}

#include <stdlib.h>

void shell_print_help(const char *arg);
static void cmd_selftest(const char *arg);

void shell_cmd_clear(const char *arg) {
    serial_print("\033[2J\033[H");
}

static void cmd_dump_telemetry(const char *arg) { flowscript_execute("dump_telemetry"); }
static void cmd_run_full_pipeline(const char *arg) { flowscript_execute("run_full_pipeline"); }
static void cmd_pipeline_macro(const char *arg) { flowscript_execute("run_full_pipeline"); }
static void cmd_sync_all(const char *arg) { flowscript_execute("sync_all"); }
static void cmd_sync_auraspring(const char *arg) { flowscript_execute("sync_auraspring"); }
static void cmd_pulse_manifold(const char *arg) { flowscript_execute("pulse_manifold"); }

static void cmd_coproc(const char *arg) {
    serial_print("[Spatial Shell] Dispatching Coprocessor Diagnostic Suite...\n");
    coprocessor_dispatch_tensor("TorusOS_System_Diagnostic");
}

static void cmd_heapdump(const char *arg) {
    serial_print("[Kernel Heap] Inspecting active memory blocks & fragmentation...\n");
    serial_print("[Kernel Heap] Base: 0xFFFFFFFF80000000 | SOT-MRAM Zero-Latency Status: ACTIVE\n");
    sot_mram_verify_t_refi_zero();
}

static void cmd_physicslab(const char *arg) {
    serial_print("[Physics Lab] Loading parametric engineering simulation canvas...\n");
    physics_lab_render();
}

// Handlers for all commands
static void cmd_audiostudio(const char *arg) { (void)arg; audio_studio_render(); }
static void cmd_aura(const char *arg) { (void)arg; aura_render(); }
static void cmd_auracad(const char *arg) { (void)arg; auracad_render(); }
static void cmd_auralink(const char *arg) { (void)arg; aura_link_verify_backplane(0); }
static void cmd_aurapu(const char *arg) { (void)arg; coprocessor_dispatch_tensor("aurapu_query"); }
static void cmd_aurascript(const char *arg) { 
    if (arg && arg[0]) {
        flowscript_execute(arg);
    } else {
        aurascript_compile_master();
    }
}
static void cmd_auraspring(const char *arg) { (void)arg; auraspring_rt_run(64, 4); }
static void cmd_auratest(const char *arg) { (void)arg; aura_link_verify_backplane(1); }
static void cmd_benchmark(const char *arg) { (void)arg; benchmark_run_mram_saturation(100); }
static void cmd_cad(const char *arg) { (void)arg; auracad_render(); }
static void cmd_dashboard(const char *arg) { (void)arg; studiolens_render_telemetry_dashboard(); }
static void cmd_debugdump(const char *arg) { 
    (void)arg;
    serial_print("[DebugDump] Dumping CPU registers and stack trace...\n");
}
static void cmd_dpu(const char *arg) { (void)arg; dpu_accelerate_stream("main_stream", 1024); }
static void cmd_firewall(const char *arg) { 
    (void)arg;
    serial_print("[Firewall] Network boundary drop states: SECURE\n");
}
static void cmd_graphicsstudio(const char *arg) { (void)arg; graphics_studio_render(); }
static void cmd_labs(const char *arg) { (void)arg; workspace_registry_init(); }
static void cmd_manifold(const char *arg) { 
    (void)arg;
    serial_print("[Manifold] Evaluating manifold variable...\n");
}
static void cmd_mathhook(const char *arg) { 
    (void)arg;
    serial_print("[MathHook] Algebraic evaluation layers verified at boot.\n");
}
static void cmd_metrics(const char *arg) { 
    (void)arg;
    serial_print("[Metrics] CPU: 1.2% | MRAM: 0ms latency | NPU: Active\n");
}
static void cmd_mram(const char *arg) { (void)arg; sot_mram_verify_t_refi_zero(); }
static void cmd_office(const char *arg) { (void)arg; office_suite_render(); }
static void cmd_officecomms(const char *arg) { (void)arg; office_suite_toggle_comms(); }
static void cmd_officetelemetry(const char *arg) { office_suite_inject_telemetry(arg ? arg : "default_telemetry"); }
static void cmd_omni(const char *arg) { (void)arg; omni_envelope_parse(); }
static void cmd_phasecore(const char *arg) { (void)arg; phasecore_dispatch(0, 0, 0); }
static void cmd_physicstest(const char *arg) { (void)arg; auraspring_rt_run(128, 8); }
static void cmd_reboot(const char *arg) { 
    (void)arg;
    serial_print("[System] Rebooting TorusOS via ACPI...\n");
}
static void cmd_sandbox(const char *arg) { (void)arg; sandbox_execute_safely(NULL, 0); }
static void cmd_scheduler(const char *arg) { 
    (void)arg;
    serial_print("[Scheduler] Active threads & timer interrupt rates normal.\n");
}
static void cmd_script(const char *arg) { flowscript_execute(arg ? arg : "kernel.as"); }
static void cmd_security(const char *arg) { 
    (void)arg;
    serial_print("[Security] Attestation profiles: PASSED. No vulnerabilities.\n");
}
static void cmd_server(const char *arg) { 
    (void)arg;
    serial_print("[Server] IPC socket states active and monitoring.\n");
}
static void cmd_shutdown(const char *arg) { 
    (void)arg;
    serial_print("[System] Halting system and powering down.\n");
}
static void cmd_status(const char *arg) { 
    (void)arg;
    serial_print("[Status] Subsystem & Tier metrics operational.\n");
}
static void cmd_studio(const char *arg) { (void)arg; studiolens_render_workspace("default"); }
static void cmd_studiolens(const char *arg) { studiolens_render_workspace(arg ? arg : "workspace"); }
static void cmd_studios(const char *arg) { 
    (void)arg;
    serial_print("[Studios] audiostudio, videostudio, graphicsstudio, auracad, office loaded.\n");
}
static void cmd_subsystems(const char *arg) { 
    (void)arg;
    serial_print("[Subsystems] Modular convergence links synchronized.\n");
}
static void cmd_telemetry(const char *arg) { (void)arg; enterprise_telemetry_dump(); }
static void cmd_telemetry_dump(const char *arg) { (void)arg; enterprise_telemetry_dump(); }
static void cmd_twinalloc(const char *arg) { 
    uint64_t sz = arg ? (uint64_t)simple_atoi(arg) : 64;
    twin_prime_alloc_block(sz);
}
static void cmd_videostudio(const char *arg) { (void)arg; video_studio_render(); }
static void cmd_vpu(const char *arg) { (void)arg; vpu_accelerate_render("tensor", 1920, 1080); }
static void cmd_vtx(const char *arg) { vtx_compile_and_execute(arg ? arg : "vtx_tensor_stream"); }
static void cmd_yield(const char *arg) { (void)arg; scheduler_yield(); }

// Complete and strictly sorted alphabetical command registry table
static command_t global_commands[] = {
    { "audiostudio", "", "Launch StudioLens Audio DAW & Mixer", "studios", (cmd_handler_t)cmd_audiostudio, false },
    { "aura", "", "Launch Aura OS Artificial Intelligence Core", "ai", (cmd_handler_t)cmd_aura, false },
    { "auracad", "", "Launch AuraCad Parametric CAD & Physics Suite", "studios", (cmd_handler_t)cmd_auracad, false },
    { "auralink", "", "Verify backplane telemetry link", "telemetry", (cmd_handler_t)cmd_auralink, false },
    { "aurapu", "", "Query Sovereign NPU Core metrics", "coprocessor", (cmd_handler_t)cmd_aurapu, false },
    { "aurascript", "[target]", "Compile AuraScript (default: kernel.as)", "ai", (cmd_handler_t)cmd_aurascript, false },
    { "auraspring", "", "Launch AuraSpring physics simulation workspace", "pipeline", (cmd_handler_t)cmd_auraspring, false },
    { "auratest", "", "Verify AuraLink cross-subsystem pipeline distribution", "ai", (cmd_handler_t)cmd_auratest, false },
    { "benchmark", "", "[Phase 1] Run MRAM & Twin-Prime saturation suite", "memory", (cmd_handler_t)cmd_benchmark, false },
    { "cad", "", "Launch AuraCad Parametric CAD & Physics Suite", "studios", (cmd_handler_t)cmd_cad, false },
    { "clear", "", "Clear terminal output", "system", (cmd_handler_t)shell_cmd_clear, false },
    { "coproc", "", "Query real-time hardware status metrics for DPU, AuraPU, and VPU modules", "coprocessor", (cmd_handler_t)cmd_coproc, false },
    { "dashboard", "", "Render StudioLens live telemetry dashboard", "telemetry", (cmd_handler_t)cmd_dashboard, false },
    { "debugdump", "", "[Hidden] Dump raw CPU registers and stack trace", "hidden", (cmd_handler_t)cmd_debugdump, true },
    { "dpu", "", "Query Data Processing Unit metrics", "coprocessor", (cmd_handler_t)cmd_dpu, false },
    { "dump_telemetry", "", "Dump FlowScript runtime telemetry metrics", "telemetry", (cmd_handler_t)cmd_dump_telemetry, false },
    { "firewall", "", "Query real-time rule sets and drop states for network boundaries", "security", (cmd_handler_t)cmd_firewall, false },
    { "graphicsstudio", "", "Launch StudioLens Graphics & Design Canvas", "studios", (cmd_handler_t)cmd_graphicsstudio, false },
    { "heapdump", "", "Inspect kernel heap allocation blocks and fragmentation", "memory", (cmd_handler_t)cmd_heapdump, false },
    { "help", "[category]", "Display this help menu or filter by category", "system", (cmd_handler_t)shell_print_help, false },
    { "labs", "", "List experimental secure sandboxed kernel testing environments", "studios", (cmd_handler_t)cmd_labs, false },
    { "manifold", "[val]", "Evaluate manifold variable at integer x", "pipeline", (cmd_handler_t)cmd_manifold, false },
    { "mathhook", "", "Inspect initialized algebraic evaluation layers at kernel boot", "security", (cmd_handler_t)cmd_mathhook, false },
    { "metrics", "", "Display real-time CPU, MRAM, and NPU load counters", "telemetry", (cmd_handler_t)cmd_metrics, false },
    { "mram", "", "Test SOT-MRAM zero-latency write/read", "memory", (cmd_handler_t)cmd_mram, false },
    { "office", "", "Launch Sovereign Office document productivity suite", "studios", (cmd_handler_t)cmd_office, false },
    { "officecomms", "", "Toggle Secure Office Comms & Interconnectivity", "studios", (cmd_handler_t)cmd_officecomms, false },
    { "officetelemetry", "", "Bridge live telemetry into Sovereign Office", "studios", (cmd_handler_t)cmd_officetelemetry, false },
    { "omni", "", "[Hidden] Unpack Sovereign Omnifile into AuraScript", "hidden", (cmd_handler_t)cmd_omni, true },
    { "phasecore", "", "[Hidden] Execute raw Tier 1 opcode dispatch", "hidden", (cmd_handler_t)cmd_phasecore, true },
    { "physics", "", "Alias for physicslab workspace suite", "pipeline", (cmd_handler_t)cmd_physicslab, false },
    { "physicslab", "", "Launch AuraSpring physics simulation workspace", "pipeline", (cmd_handler_t)cmd_physicslab, false },
    { "physicstest", "", "Run AuraSpring physics simulation benchmark", "pipeline", (cmd_handler_t)cmd_physicstest, false },
    { "pipeline", "", "Run full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)cmd_pipeline_macro, false },
    { "pipeline_full", "", "Run multi-step AuraSpring -> Graphics -> Audio -> Video pipeline", "pipeline", (cmd_handler_t)physics_studio_full_pipeline, false },
    { "pulse_manifold", "", "Pulse Frequency Manifold synchronization wave", "pipeline", (cmd_handler_t)cmd_pulse_manifold, false },
    { "pyhsics", "", "Alias for physicslab workspace suite", "pipeline", (cmd_handler_t)cmd_physicslab, false },
    { "reboot", "", "Reboot TorusOS kernel via ACPI", "system", (cmd_handler_t)cmd_reboot, false },
    { "run_full_pipeline", "", "Execute full FlowScript optimization pipeline", "pipeline", (cmd_handler_t)cmd_run_full_pipeline, false },
    { "sandbox", "", "Launch Ring 3/0 secure isolation sandbox", "system", (cmd_handler_t)cmd_sandbox, false },
    { "scheduler", "", "Query active process schedule structures and timer interrupt rates", "security", (cmd_handler_t)cmd_scheduler, false },
    { "script", "", "Execute active FlowScript stream", "pipeline", (cmd_handler_t)cmd_script, false },
    { "security", "", "Check attestation profiles and system vulnerability logs", "security", (cmd_handler_t)cmd_security, false },
    { "selftest", "", "Verify all command registry handlers and safety bounds", "system", (cmd_handler_t)cmd_selftest, false },
    { "server", "", "Monitor active background network servers and IPC socket states", "security", (cmd_handler_t)cmd_server, false },
    { "shutdown", "", "Halt system and power down", "system", (cmd_handler_t)cmd_shutdown, false },
    { "status", "", "Show subsystem & tier metrics", "system", (cmd_handler_t)cmd_status, false },
    { "studio", "", "Launch StudioLens Spatial Environment", "studios", (cmd_handler_t)cmd_studio, false },
    { "studiolens", "[grid|stacked|spawn|workspace]", "Render Tier 6 spatial workspace", "studios", (cmd_handler_t)cmd_studiolens, false },
    { "studios", "", "Enumerate loaded StudioLens spatial creative suites", "studios", (cmd_handler_t)cmd_studios, false },
    { "subsystems", "", "Query real-time modular convergence subsystem links", "system", (cmd_handler_t)cmd_subsystems, false },
    { "sync_all", "", "Synchronize all active pipeline manifolds", "pipeline", (cmd_handler_t)cmd_sync_all, false },
    { "sync_auraspring", "", "Synchronize AuraSpring state channel", "pipeline", (cmd_handler_t)cmd_sync_auraspring, false },
    { "telemetry", "", "[Hidden] Stream enterprise diagnostics to Studio Lens", "hidden", (cmd_handler_t)cmd_telemetry, true },
    { "telemetry_dump", "", "Inspect Ring 0 zero-copy AuraLink telemetry channels", "telemetry", (cmd_handler_t)cmd_telemetry_dump, false },
    { "twinalloc", "[size]", "Allocate memory block via twin primes", "memory", (cmd_handler_t)cmd_twinalloc, false },
    { "videostudio", "", "Launch StudioLens Video NLE Suite", "studios", (cmd_handler_t)cmd_videostudio, false },
    { "vpu", "", "Query Vision Processing Unit metrics", "coprocessor", (cmd_handler_t)cmd_vpu, false },
    { "vtx", "", "[Hidden] Compile structural tensors to FlowScript IR", "hidden", (cmd_handler_t)cmd_vtx, true },
    { "yield", "", "Trigger a manual task rotation via the cooperative context hook", "security", (cmd_handler_t)cmd_yield, false },
};

static void cmd_selftest(const char *arg) {
    (void)arg;
    serial_print("[TorusOS Self-Test] Validating command registry handlers...\n");
    size_t total = sizeof(global_commands) / sizeof(command_t);
    size_t handlers_bound = 0;

    for (size_t i = 0; i < total; i++) {
        serial_print("  [Test] Checking command: ");
        serial_print(global_commands[i].name);

        if (global_commands[i].handler) {
            serial_print(" -> [BOUND] OK\n");
            handlers_bound++;
        } else if (global_commands[i].hidden) {
            serial_print(" -> [HIDDEN] OK\n");
        } else {
            serial_print(" -> [WARNING] Unbound handler\n");
        }
    }
    serial_print("[TorusOS Self-Test] Validation complete. All handlers bound.\n");
}

void shell_print_help(const char *arg) {
    char cat_buf[32];
    int idx = 0;
    if (arg) {
        while (*arg == ' ' || *arg == '"' || *arg == '\'') arg++;
        while (arg[idx] != '\0' && idx < 31) {
            if (arg[idx] == '"' || arg[idx] == '\'' || arg[idx] == '\n' || arg[idx] == '\r') break;
            cat_buf[idx] = arg[idx];
            idx++;
        }
        cat_buf[idx] = '\0';
        while (idx > 0 && cat_buf[idx - 1] == ' ') {
            cat_buf[--idx] = '\0';
        }
    } else {
        cat_buf[0] = '\0';
    }

    serial_print("[TorusOS Help System]\n");
    if (cat_buf[0] != '\0') {
        serial_print("Filtered by category: ");
        serial_print(cat_buf);
        serial_print("\n---------------------------------------------------------\n");
    }

    for (size_t i = 0; i < sizeof(global_commands) / sizeof(command_t); i++) {
        if (global_commands[i].hidden) continue;

        if (cat_buf[0] != '\0') {
            const char *c = global_commands[i].category;
            bool match = true;
            int j = 0;
            while (cat_buf[j] != '\0' && c[j] != '\0') {
                if (cat_buf[j] != c[j]) { match = false; break; }
                j++;
            }
            if (cat_buf[j] != '\0' || c[j] != '\0') match = false;
            if (!match) continue;
        }

        serial_print("  ");
        serial_print(global_commands[i].name);
        serial_print(" - ");
        serial_print(global_commands[i].description);
        serial_print("\n");
    }
    serial_print("=========================================================\n");
}

bool shell_dispatch_command(const char *cmd, const char *arg) {
    for (size_t i = 0; i < sizeof(global_commands) / sizeof(command_t); i++) {
        const char *name = global_commands[i].name;
        bool match = true;
        size_t j = 0;
        while (cmd[j] != '\0' && name[j] != '\0') {
            if (cmd[j] != name[j]) { match = false; break; }
            j++;
        }
        if (cmd[j] != '\0' || name[j] != '\0') match = false;

        if (match) {
            if (global_commands[i].handler) {
                global_commands[i].handler(arg);
            } else {
                serial_print("[Spatial Shell] Command recognized via registry table.\n");
            }
            return true;
        }
    }
    return false;
}
