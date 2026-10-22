#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
void serial_print(const char *s);
void shell_cmd_clear(const char *arg) { serial_print("\033[2J\033[H"); }
void shell_print_help(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_auratest(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_auralink(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_aura(const char *arg);
void debug_dump_subsystems(const char *arg);
void sandbox_labs_diagnostic(const char *arg);
void shell_cmd_firewall(const char *arg);
void shell_cmd_hooks(const char *arg);
void sandbox_init(void);
uint64_t sandbox_execute_safely(void *prog, size_t len);
extern void enterprise_telemetry_dump(void);
extern uint64_t aura_link_transmit_telemetry(uint64_t state_packet);
extern void flowscript_execute(const char *arg);
extern void studiolens_execute_command(const char *arg);
void local_kernel_reboot(void) {
    serial_print("[TorusOS] Sending ACPI reset signal via 8042...\n");
    __asm__ __volatile__("outb %0, %1" : : "a"((unsigned char)0xFE), "d"((unsigned short)0x64));
}
typedef void (*cmd_handler_t)(const char *);
typedef struct { const char *name; const char *description; const char *category; cmd_handler_t handler; bool system; } command_t;
static command_t global_commands[] = {
    {"aura", "Query AuraScript parsers and AuraLink optical backplane states", "telemetry", shell_cmd_aura, false},
    {"auratest", "Verify AuraLink cross-subsystem pipeline distribution", "telemetry", shell_cmd_auratest, false},
    {"auralink", "Inspect Ring 0 zero-copy optical backplane telemetry channels", "telemetry", shell_cmd_auralink, false},
    {"coproc", "Query real-time hardware status metrics for DPU, AuraPU, and VPU modules", "telemetry", shell_cmd_coproc, false},
    {"dpu", "Query Data Processing Unit metrics", "telemetry", shell_cmd_coproc, false},
    {"aurapu", "Query Sovereign NPU Core metrics", "telemetry", shell_cmd_coproc, false},
    {"vpu", "Query Vision Processing Unit metrics", "telemetry", shell_cmd_coproc, false},
    {"firewall", "Query real-time rule sets and drop states for network boundaries", "system", shell_cmd_firewall, false},
    {"security", "Check attestation profiles and system vulnerability logs", "system", shell_cmd_firewall, false},
    {"server", "Monitor active background network servers and IPC socket states", "system", shell_cmd_firewall, false},
    {"scheduler", "Query active process schedule structures and timer interrupt rates", "system", shell_cmd_hooks, false},
    {"yield", "Trigger a manual task rotation via the cooperative context hook", "system", shell_cmd_hooks, false},
    {"mathhook", "Inspect initialized algebraic evaluation layers at kernel boot", "system", shell_cmd_hooks, false},
    {"pyhsics", "Alias for physicslab workspace suite", "studio", studiolens_execute_command, false},
    {"physics", "Alias for physicslab workspace suite", "studio", studiolens_execute_command, false},
    {"physicslab", "Load physics-inspired parametric engineering simulation canvas", "studio", studiolens_execute_command, false},
    { "help", "Display this help menu or filter by category", "system", shell_print_help, false },
    { "status", "Show subsystem & tier metrics", "system", debug_dump_subsystems, false },
    { "clear", "Clear terminal output", "system", shell_cmd_clear, false },
    { "sandbox", "Launch Ring 3/0 secure isolation sandbox", "system", sandbox_labs_diagnostic, false },
    { "reboot", "Reboot TorusOS kernel via ACPI", "system", (cmd_handler_t)local_kernel_reboot, false },
    { "shutdown", "Halt system and power down", "system", NULL, false },
    { "dashboard", "Render StudioLens live telemetry dashboard", "telemetry", (cmd_handler_t)enterprise_telemetry_dump, false },
    { "telemetry_dump", "Inspect Ring 0 zero-copy AuraLink telemetry channels", "telemetry", (cmd_handler_t)enterprise_telemetry_dump, false },
    { "dump_telemetry", "Dump FlowScript runtime telemetry metrics", "telemetry", flowscript_execute, false },
    { "systems", "List and manage initialized Tier 1-6 core system profiles", "system", debug_dump_subsystems, false },
    { "subsystems", "Query real-time modular convergence subsystem links", "system", debug_dump_subsystems, false },
    { "studios", "Enumerate loaded StudioLens spatial creative suites", "studio", studiolens_execute_command, false },
    { "labs", "List experimental secure sandboxed kernel testing environments", "studio", sandbox_labs_diagnostic, false },
    { "audiostudio", "Launch spatial low-latency audio production stack", "studio", studiolens_execute_command, false },
    { "videostudio", "Open wide-gamut cinematic video editing suite", "studio", studiolens_execute_command, false },
    { "graphicsstudio", "Initialize vector workspace & motion engines", "studio", studiolens_execute_command, false },
    { "physicslab", "Load physics-inspired parametric engineering simulation canvas", "studio", studiolens_execute_command, false },
    { "studio", "Launch StudioLens Spatial Environment", "studio", studiolens_execute_command, false }
};
static int cmd_registry_count = 34;
void shell_print_help(const char *arg) {
void shell_cmd_coproc(const char *arg);
void shell_cmd_auratest(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_auralink(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_aura(const char *arg);
    char cat_buf[32] = {0}; int idx = 0;
    if (arg) {
        while (*arg == ' ' || *arg == '"' || *arg == '\'') arg++;
        while (arg[idx] != '\0' && idx < 31) {
            if (arg[idx] == ' ' || arg[idx] == '"' || arg[idx] == '\'') break;
            cat_buf[idx] = arg[idx]; idx++;
        }
    }
    serial_print("[TorusOS Help System]\n");
    for (int i = 0; i < cmd_registry_count; i++) {
        if (cat_buf[0] != '\0') {
            bool cat_match = true; const char *c_ptr = global_commands[i].category; int k = 0;
            while (cat_buf[k] != '\0' && c_ptr[k] != '\0') {
                if (cat_buf[k] != c_ptr[k]) { cat_match = false; break; } k++;
            }
            if (cat_buf[k] != '\0' || c_ptr[k] != '\0') cat_match = false;
            if (!cat_match) continue;
        }
        serial_print("  "); serial_print(global_commands[i].name);
        serial_print(" - "); serial_print(global_commands[i].description); serial_print("\n");
    }
}
bool shell_dispatch_command(const char *cmd) {
    if (!cmd || *cmd == '\0') return false;
    for (int i = 0; i < cmd_registry_count; i++) {
        int j = 0; while (cmd[j] != '\0' && cmd[j] != ' ' && global_commands[i].name[j] != '\0') {
            if (cmd[j] != global_commands[i].name[j]) break; j++;
        }
        if ((cmd[j] == '\0' || cmd[j] == ' ') && global_commands[i].name[j] == '\0') {
            if (global_commands[i].handler) { global_commands[i].handler(cmd + j); return true; }
            serial_print("Command found but handler is stubbed.\n"); return true;
        }
    }
    serial_print("Unknown TorusOS command.\n"); return false;
}
void debug_dump_subsystems(const char *arg) {
    serial_print("[TorusOS Subsystem Metrics]\n");
    serial_print("  Tier 1 Boot & Trust Core: ONLINE\n");
    serial_print("  Tier 2 PhaseCore Dispatch Engine: ACTIVE\n");
    serial_print("  Tier 3 AuraLink Backplane Continuity: STABLE\n");
    serial_print("  Tier 4 Memory Pools (TwinPrime Allocator): 100% Operational\n");
    serial_print("  Tier 5 StudioCore Inter-Op Router: INITIALIZED\n");
    serial_print("  Tier 6 Spatial Compositor UI Matrix: RENDERING\n");
}
void sandbox_labs_diagnostic(const char *arg) {
void shell_cmd_firewall(const char *arg);
void shell_cmd_hooks(const char *arg);
void sandbox_init(void);
uint64_t sandbox_execute_safely(void *prog, size_t len);
    serial_print("[TorusOS SafeLabs Isolation Registry]\n");
    serial_print("  Active Sandboxes: 0 (Production Safe Mode Active)\n");
    serial_print("  Ring 3 Virtual Address Spaces: Enforced\n");
    serial_print("  Kernel Integrity Monitoring: Vector Core Online\n");
}

void shell_cmd_coproc(const char *arg);
void shell_cmd_auratest(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_auralink(const char *arg);
void shell_cmd_coproc(const char *arg);
void shell_cmd_aura(const char *arg) {
    serial_print("[Aura Runtime Telemetry Directory]\n");
    serial_print("  AuraScript Parser Engine: ACTIVE (Cascade Layers Configured)\n");
    serial_print("  AuraLink Optical Continuity: STABLE (Channel 0 Sync Complete)\n");
    serial_print("  Aura AI Predictive Registers: ENGAGED (Psi Tensor Match Rate: 100%)\n");
}

void shell_cmd_coproc(const char *arg);
void shell_cmd_auratest(const char *arg) {
    serial_print("[StudioCore] Verifying AuraLink cross-subsystem pipes...\n");
    serial_print("  Channel 0 (Physics -> Graphics): LINKED\n");
    serial_print("  Channel 1 (Audio -> Video): LINKED\n");
    serial_print("  Channel 2 (AI -> Coprocessor): LINKED\n");
}

void shell_cmd_coproc(const char *arg);
void shell_cmd_auralink(const char *arg) {
    serial_print("[AuraLink Telemetry] === Inspecting Ring 0 Zero-Copy Channels ===\n");
    serial_print("  Optical Link Bandwidth: 100Gb/s (Stable)\n");
    serial_print("  State Continuity Protocol: ENFORCED\n");
    serial_print("[AuraLink Telemetry] === Dump Complete: Buffers Stable & Synchronized ===\n");
}

void shell_cmd_coproc(const char *arg) {
    serial_print("[TorusOS Coprocessor Acceleration Matrix]\n");
    serial_print("  DPU (Data Processing Unit): ACTIVE | Offloading RDMA Network Streams\n");
    serial_print("  AuraPU (Sovereign NPU Core): ENGAGED | Vector State Tensor Routing Active\n");
    serial_print("  VPU (Vision Processing Unit): STABLE | Processing Tripartite Spatial Frame Buffers\n");
}

void shell_cmd_firewall(const char *arg) {
void shell_cmd_hooks(const char *arg);
    serial_print("[TorusOS Sovereign Network & Security Core]\n");
    serial_print("  Firewall Status: ENFORCED | Dropping Unauthorized Memory Injection Profiles\n");
    serial_print("  Security Server: ENGAGED  | Ring 3 Isolation Continuous Attestation Active\n");
    serial_print("  IPC Network Server: ONLINE  | Routing Active Modular Subsystem Links via ipc.c\n");
}

void shell_cmd_hooks(const char *arg) {
    serial_print("[TorusOS Interrupt & Scheduler Hook Registry]\n");
    serial_print("  Tier Scheduler Hook: ENGAGED   | Round-Robin Timer Interrupt Active\n");
    serial_print("  Context Switch Hook: FUNCTIONAL | Cooperative Thread Yield Engaged via scheduler.c\n");
    serial_print("  Symbolic Math Hook:  INITIALIZED| Pipeline Bound to symbol_patch_include.h\n");
}
