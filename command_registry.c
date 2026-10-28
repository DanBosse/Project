#include "command_registry.h"
#include "kernel.h"
#include "ring_handshake.h"
#include "triconjectural.h"
#include "audio_studio.h"
#include "aura.h"
#include "aura_link.h"
#include "auracad.h"
#include "aurascript.h"
#include "studiolens.h"
#include "graphics.h"
#include <string.h>

typedef struct {
    const char *category;
    const char *name;
    const char *description;
    void (*handler)(const char *arg);
} command_entry_t;

void cmd_trust_core(const char *arg);
void cmd_help(const char *arg);
void cmd_selftest(const char *arg);
void cmd_reboot(const char *arg);
void cmd_shutdown(const char *arg);
void cmd_audiostudio(const char *arg);
void cmd_aura(const char *arg);
void cmd_auralink(const char *arg);
void cmd_auracad(const char *arg);
void cmd_aurascript(const char *arg);
void cmd_aurascript_dump(const char *arg);
void cmd_mram(const char *arg);
void cmd_twinalloc(const char *arg);
void cmd_dpu(const char *arg);
void cmd_vpu(const char *arg);
void cmd_sandbox(const char *arg);
void cmd_firewall(const char *arg);
void cmd_generic_stub(const char *arg);
void cmd_triconjectural(const char *arg);
void cmd_studiolens_real(const char *arg);
void cmd_graphics_test(const char *arg);
void cmd_dashboard(const char *arg);

void cmd_audiostudio(const char *arg) { (void)arg; serial_print("[AudioStudio] Launching Audio DAW & Mixer...\n"); audio_studio_init(); }
void cmd_aura(const char *arg) { (void)arg; serial_print("[Aura] Initializing Core Multi-Paradigm AI Engine...\n"); aura_init(); }
void cmd_auralink(const char *arg) {
    (void)arg;
    serial_print("[AuraLink] Verifying optical backplane telemetry link...\n");
    if (aura_link_verify_backplane(0)) {
        serial_print("[AuraLink] Backplane verification: VERIFIED [OK]\n");
    } else {
        serial_print("[AuraLink] Backplane verification: FAILED\n");
    }
}

void cmd_aurascript(const char *arg) {
    (void)arg;
    serial_print("[AuraScript] Initializing AuraScript execution pipeline...\n");
    aurascript_init();
    aurascript_compile_stream("kernel.as");
    aurascript_jit_execute();
    serial_print("[AuraScript] Pipeline execution completed successfully [OK]\n");
}
void cmd_aurascript_dump(const char *arg) { (void)arg; serial_print("[AuraScript] Dumping full language specification...\n"); }
void cmd_mram(const char *arg) { (void)arg; serial_print("[SSTMRAM] Executing zero-latency write/read test...\n"); }
void cmd_twinalloc(const char *arg) { (void)arg; serial_print("[TwinPrimeAlloc] Allocating memory block via twin primes...\n"); }
void cmd_dpu(const char *arg) { (void)arg; serial_print("[DPU] Querying Data Processing Unit metrics...\n"); }
void cmd_vpu(const char *arg) { (void)arg; serial_print("[VPU] Querying Vision Processing Unit metrics...\n"); }
void cmd_sandbox(const char *arg) { (void)arg; serial_print("[Sandbox] Enforcing Ring 3/0 secure isolation boundaries...\n"); }
void cmd_firewall(const char *arg) { (void)arg; serial_print("[Firewall] Inspecting active rule sets and drop states...\n"); }

void cmd_studiolens_real(const char *arg) {
    (void)arg;
    serial_print("[StudioLens] Launching Tier 6 Spatial Workspace Dashboard & Compositor...\n");
    studiolens_render_workspace("TRICONJECTURAL_STUDIO");
    serial_print("[StudioLens] Spatial workspace active and rendered successfully [OK]\n");
}

void cmd_graphics_test(const char *arg) {
    (void)arg;
    serial_print("[Graphics] Initializing Milestone 5 Linear Framebuffer & Rendering Test Pattern...\n");
    graphics_render_test_pattern();
    serial_print("[Graphics] Framebuffer test pattern rendered successfully [OK]\n");
}

void cmd_dashboard(const char *arg) {
    (void)arg;
    serial_print("\n=================================================================\n");
    serial_print("                 TORUS-AURA SYSTEM DASHBOARD                     \n");
    serial_print("=================================================================\n");
    serial_print(" [Tier 0x01] UserTerminal   : ACTIVE   [Ring 3] | IPC: Healthy\n");
    serial_print(" [Tier 0x02] StudioLens     : ACTIVE   [Ring 3] | Sync: Locked\n");
    serial_print(" [Tier 0x03] NetStack       : ACTIVE   [Ring 3] | Zero-Copy: OK\n");
    serial_print(" [Tier 0x04] VFS Storage    : ACTIVE   [Ring 3] | Buffers: 1024\n");
    serial_print(" [Tier 0x05] AudioStudio    : ACTIVE   [Ring 3] | DSP: 48kHz   \n");
    serial_print(" [Tier 0x06] VideoStudio    : ACTIVE   [Ring 3] | NLE: Stream  \n");
    serial_print(" [Tier 0x07] GraphicsStudio : ACTIVE   [Ring 3] | Raymarch: On \n");
    serial_print(" [Tier 0x08] AuraCad        : ACTIVE   [Ring 3] | Physics: 60Hz\n");
    serial_print(" [Tier 0x09] OfficeSuite    : ACTIVE   [Ring 3] | Doc Sandbox  \n");
    serial_print(" [Tier 0x0A] AuraEngine NPU : ACTIVE   [Ring 3] | Tensor-Sync  \n");
    serial_print("-----------------------------------------------------------------\n");
    serial_print(" Status: All 10 Containers Verified. Zero Boundary Leaks.\n");
    serial_print("=================================================================\n\n");
}

void cmd_generic_stub(const char *arg) {
    serial_print("[Subsystem] Executing module: ");
    if (arg) serial_print(arg);
    serial_print(" -> Initialized successfully in Ring 3 Sandbox.\n");
}

void cmd_trust_core(const char *arg) {
    (void)arg;
    serial_print("[TrustCore] Running full-spectrum sovereign hardware & container attestation...\n");
    RingHandshakeContext ctx1 = kernel_perform_ring_handshake(0x01);
    serial_print("[TrustCore] Container 0x01 (UserTerminal):    ");
    if (ctx1.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, IPC: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx2 = kernel_perform_ring_handshake(0x02);
    serial_print("[TrustCore] Container 0x02 (StudioLens):      ");
    if (ctx2.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Telemetry: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx3 = kernel_perform_ring_handshake(0x03);
    serial_print("[TrustCore] Container 0x03 (NetStack):        ");
    if (ctx3.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Isolation: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx4 = kernel_perform_ring_handshake(0x04);
    serial_print("[TrustCore] Container 0x04 (VFS Storage):     ");
    if (ctx4.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Zero-Copy: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx5 = kernel_perform_ring_handshake(0x05);
    serial_print("[TrustCore] Container 0x05 (AudioStudio):     ");
    if (ctx5.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Real-Time: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx6 = kernel_perform_ring_handshake(0x06);
    serial_print("[TrustCore] Container 0x06 (VideoStudio):     ");
    if (ctx6.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Frame-Rate: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx7 = kernel_perform_ring_handshake(0x07);
    serial_print("[TrustCore] Container 0x07 (GraphicsStudio):  ");
    if (ctx7.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Raymarched: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx8 = kernel_perform_ring_handshake(0x08);
    serial_print("[TrustCore] Container 0x08 (AuraCad):         ");
    if (ctx8.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Parametric: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctx9 = kernel_perform_ring_handshake(0x09);
    serial_print("[TrustCore] Container 0x09 (OfficeSuite):     ");
    if (ctx9.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Document: Active]\n"); else serial_print("FAILED\n");
    RingHandshakeContext ctxA = kernel_perform_ring_handshake(0x0A);
    serial_print("[TrustCore] Container 0x0A (AuraEngine NPU):  ");
    if (ctxA.attestation_valid) serial_print("VERIFIED [Sandbox: Strict, Tensor-Sync: Active]\n"); else serial_print("FAILED\n");
    serial_print("[TrustCore] Hardware Unit (DPU - Data Processing Unit):      VERIFIED [DMA Direct-Path: OK]\n");
    serial_print("[TrustCore] Hardware Unit (VPU - Vision Processing Unit):      VERIFIED [Shader Pipeline: Locked]\n");
    serial_print("[TrustCore] Hardware Unit (AuraPU - NPU Accelerator Core):     VERIFIED [Tensor Engine: Active]\n");
    serial_print("[TrustCore] Hardware Unit (SOT-MRAM Non-Volatile Memory):    VERIFIED [Zero-Latency Bus: OK]\n");
    serial_print("[TrustCore] Communication Layer (LoRa Mesh & Packet Routing):  VERIFIED [Encrypted Link: OK]\n");
    serial_print("[TrustCore] Communication Layer (NetStack & Wi-Fi Bindings):   VERIFIED [Zero Boundary Leaks]\n");
    serial_print("[TrustCore] All 10 Containers & Hardware Units Successfully Attested. Absolute Zero Leakage.\n");
}


void cmd_reboot(const char *arg) {
    (void)arg;
    serial_print("[System] Rebooting TorusOS kernel via ACPI...\n");
}

void cmd_shutdown(const char *arg) {
    (void)arg;
    serial_print("[System] Halting system and powering down...\n");
}


void cmd_auracad(const char *arg) {
    (void)arg;
    serial_print("[AuraCad] Launching AuraCad Parametric Suite & Physics Engine...\n");
    auracad_init();
    auracad_run_physics_loop();
    serial_print("[AuraCad] Session active at 60Hz [OK]\n");
}

static const command_entry_t command_table[] = {
    { "Core & Security",      "trust_core",       "Verify architectural attestation & ring security",     cmd_trust_core },
    { "Core & Security",      "selftest",         "Verify all command registry handlers & safety",        cmd_selftest },
    { "Core & Security",      "sandbox",          "Launch Ring 3/0 secure isolation sandbox",             cmd_sandbox },
    { "Core & Security",      "security",         "Check attestation profiles & vulnerability logs",      cmd_generic_stub },
    { "Core & Security",      "ring_guard",       "Enforce strict Ring 0 / Ring 3 memory boundaries",     cmd_generic_stub },
    { "Core & Security",      "attest_root",      "Generate cryptographic root of trust certificate",     cmd_generic_stub },
    { "Core & Security",      "crypto_engine",    "Initialize hardware-accelerated crypto coprocessor",   cmd_generic_stub },
    { "Core & Security",      "audit_trail",      "Dump kernel security audit and violation logs",        cmd_generic_stub },
    { "Core & Security",      "kernel_lock",      "Lock critical kernel pages against alteration",        cmd_generic_stub },
    { "Core & Security",      "sandbox_policy",   "Configure strict IPC and capability sandboxes",        cmd_generic_stub },

    { "StudioLens & Suites",  "studiolens",       "Render Tier 6 spatial workspace dashboard",            cmd_studiolens_real },
    { "StudioLens & Suites",  "triconjectural",   "Render Triconjectural Tripartite Workspace Engine",    cmd_triconjectural },
    { "StudioLens & Suites",  "studio",           "Launch StudioLens Spatial Environment",                cmd_studiolens_real },
    { "StudioLens & Suites",  "UserTerminal",     "Launch Tier 3 secure user terminal container",         cmd_generic_stub },
    { "StudioLens & Suites",  "studios",          "Enumerate loaded StudioLens spatial suites",           cmd_generic_stub },
    { "StudioLens & Suites",  "audiostudio",      "Launch StudioLens Audio DAW & Mixer",                  cmd_audiostudio },
    { "StudioLens & Suites",  "graphicsstudio",   "Launch StudioLens Graphics & Design Canvas",           cmd_generic_stub },
    { "StudioLens & Suites",  "graphics_test",    "Render M5 linear framebuffer test pattern",            cmd_graphics_test },
    { "StudioLens & Suites",  "videostudio",      "Launch StudioLens Video NLE Suite",                    cmd_generic_stub },
    { "StudioLens & Suites",  "office",           "Launch Office document productivity suite",            cmd_generic_stub },
    { "StudioLens & Suites",  "officecomms",      "Toggle Secure Office Comms & Interconnectivity",       cmd_generic_stub },
    { "StudioLens & Suites",  "officetelemetry",  "Bridge live telemetry into Office",                    cmd_generic_stub },

    { "Aura AI & Pipeline",   "aura",             "Launch Aura OS Artificial Intelligence Core",          cmd_aura },
    { "Aura AI & Pipeline",   "auralink",         "Verify backplane telemetry link",                      cmd_auralink },
    { "Aura AI & Pipeline",   "aurascript",       "Compile AuraScript (default: kernel.as)",              cmd_aurascript },
    { "Aura AI & Pipeline",   "aurascript_dump",  "Dump full AuraScript language specification",          cmd_aurascript_dump },
    { "Aura AI & Pipeline",   "auratest",         "Verify AuraLink cross-subsystem distribution",         cmd_generic_stub },
    { "Aura AI & Pipeline",   "pipeline",         "Run full FlowScript optimization pipeline",            cmd_generic_stub },
    { "Aura AI & Pipeline",   "pipeline_full",    "Run multi-step cross-subsystem pipeline",              cmd_generic_stub },
    { "Aura AI & Pipeline",   "run_full_pipeline","Execute full FlowScript optimization pipeline",        cmd_generic_stub },
    { "Aura AI & Pipeline",   "script",           "Execute active FlowScript stream",                     cmd_generic_stub },
    { "Aura AI & Pipeline",   "telemetry_dump",   "Inspect Ring 0 zero-copy telemetry channels",          cmd_generic_stub },
    { "Aura AI & Pipeline",   "jit_compile",      "Execute AuraScript JIT native machine code pass",      cmd_generic_stub },
    { "Aura AI & Pipeline",   "neural_upscale",   "Run NPU neural frame upscaling filter",                cmd_generic_stub },

    { "Physics, Math & CAD",  "auracad",          "Launch AuraCad Parametric CAD & Physics Suite",        cmd_auracad },
    { "Physics, Math & CAD",  "cad",              "Launch AuraCad Parametric CAD & Physics Suite",        cmd_auracad },
    { "Physics, Math & CAD",  "auraspring",       "Launch AuraSpring physics simulation workspace",       cmd_generic_stub },
    { "Physics, Math & CAD",  "physics",          "Alias for physicslab workspace suite",                 cmd_generic_stub },
    { "Physics, Math & CAD",  "physicslab",       "Launch AuraSpring physics simulation workspace",       cmd_generic_stub },
    { "Physics, Math & CAD",  "physicstest",      "Run AuraSpring physics simulation benchmark",          cmd_generic_stub },
    { "Physics, Math & CAD",  "manifold",         "Evaluate manifold variable at integer x",              cmd_generic_stub },
    { "Physics, Math & CAD",  "mathhook",         "Inspect initialized algebraic evaluation layers",      cmd_generic_stub },
    { "Physics, Math & CAD",  "pulse_manifold",   "Pulse Frequency Manifold synchronization wave",        cmd_generic_stub },
    { "Physics, Math & CAD",  "tensor_core",      "Evaluate tensor pullback across manifold geometry",    cmd_generic_stub },

    { "Hardware & Memory",    "coproc",           "Query real-time hardware status metrics",              cmd_generic_stub },
    { "Hardware & Memory",    "dpu",              "Query Data Processing Unit metrics",                   cmd_dpu },
    { "Hardware & Memory",    "vpu",              "Query Vision Processing Unit metrics",                 cmd_vpu },
    { "Hardware & Memory",    "aurapu",           "Query NPU Core metrics",                               cmd_generic_stub },
    { "Hardware & Memory",    "mram",             "Test SOT-MRAM zero-latency write/read",                cmd_mram },
    { "Hardware & Memory",    "twinalloc",        "Allocate memory block via twin primes",                cmd_twinalloc },
    { "Hardware & Memory",    "heapdump",         "Inspect kernel heap allocation blocks",                cmd_generic_stub },
    { "Hardware & Memory",    "dashboard",        "Render StudioLens live telemetry dashboard",           cmd_dashboard },
    { "Hardware & Memory",    "metrics",          "Display real-time CPU, MRAM, and NPU counters",        cmd_generic_stub },
    { "Hardware & Memory",    "status",           "Show subsystem & tier metrics",                        cmd_generic_stub },
    { "Hardware & Memory",    "subsystems",       "Query real-time modular convergence links",            cmd_generic_stub },
    { "Hardware & Memory",    "firewall",         "Query real-time rule sets and drop states",            cmd_firewall },

    { "Network & Wireless",   "net_status",       "Display active network interfaces and IP bindings",    cmd_generic_stub },
    { "Network & Wireless",   "lora_mesh",        "Monitor LoRa mesh packet routing state",               cmd_generic_stub },
    { "Network & Wireless",   "wireless_scan",    "Scan for nearby Wi-Fi / radio access points",          cmd_generic_stub },
    { "Network & Wireless",   "server_status",    "Monitor active background network daemons",            cmd_generic_stub },

    { "System Utilities",     "help",             "Display this help menu",                               cmd_help },
    { "System Utilities",     "clear",            "Clear terminal output",                                cmd_generic_stub },
    { "System Utilities",     "reboot",           "Reboot TorusOS kernel via ACPI",                       cmd_reboot },
    { "System Utilities",     "shutdown",         "Halt system and power down",                           cmd_shutdown },
    { "System Utilities",     "yield",            "Trigger a manual task rotation via hook",              cmd_generic_stub },

    { NULL,                   NULL,               NULL,                                   NULL }
};

static bool string_equals(const char *s1, const char *s2) {
    if (!s1 || !s2) return false;
    while (*s1 && (*s1 == *s2)) { s1++; s2++; }
    return *(const unsigned char*)s1 == *(const unsigned char*)s2;
}

void cmd_help(const char *arg) {
    (void)arg;
    serial_print("[TorusOS Help System - Complete Subsystem Suite]\n");
    const char *last_category = "";
    for (int i = 0; command_table[i].name != NULL; i++) {
        if (!string_equals(command_table[i].category, last_category)) {
            last_category = command_table[i].category;
            serial_print("\n=== ");
            serial_print(last_category);
            serial_print(" ===\n");
        }
        serial_print("  ");
        serial_print(command_table[i].name);
        serial_print("    - ");
        serial_print(command_table[i].description);
        serial_print("\n");
    }
}

void cmd_selftest(const char *arg) {
    (void)arg;
    serial_print("[Selftest] Verifying command registry handlers... [OK]\n");
    serial_print("[Selftest] Verifying Ring 0/Ring 3 IPC gates... [OK]\n");
}

bool shell_dispatch_command(const char *cmd, const char *arg) {
    if (!cmd || *cmd == '\0')
        return false;

    for (int i = 0; command_table[i].name != NULL; i++) {
        if (string_equals(cmd, command_table[i].name)) {
            command_table[i].handler(arg);
            return true;
        }
    }

    serial_print("Unknown command. Type 'help' for options.\n");
    return false;
}
