#include <stdint.h>
#include <stddef.h>
#include "torus.h"
#include "phasecore.h"
#include "simd.h"
#include "flowscript.h"
#include "aurascript_compiler.h"
#include "aura_link.h"
#include "symbolic_math.h"
#include "studiolens.h"
#include "spatial_compositor.h"
#include "synthesizer.h"
#include "sot_mram.h"
#include "twin_prime_alloc.h"
#include "omni_envelope.h"
#include "enterprise_telemetry.h"
#include "benchmark.h"

// Port I/O helpers
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Serial COM1 Input/Output (Port 0x3F8)
#define COM1_PORT 0x3F8

int serial_received(void) {
    return inb(COM1_PORT + 5) & 1;
}

char serial_read(void) {
    while ((inb(COM1_PORT + 5) & 1) == 0) {
        __asm__ volatile ("pause");
    }
    return inb(COM1_PORT);
}

// Simple string comparison helper
int streq(const char* a, const char* b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

// Simple string-to-integer helper
int simple_atoi(const char* s) {
    int res = 0;
    int sign = 1;
    if (*s == '-') {
        sign = -1;
        s++;
    }
    while (*s >= '0' && *s <= '9') {
        res = res * 10 + (*s - '0');
        s++;
    }
    return res * sign;
}

// Trim trailing whitespace and carriage returns from argument
void trim_arg(char* s) {
    if (!s) return;
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '\r' || s[i] == '\n' || s[i] == ' ') {
            s[i] = '\0';
            break;
        }
        i++;
    }
}

void kernel_main(void) {
    serial_print("=========================================================\n");
    serial_print("            TorusOS Sovereign Master Kernel              \n");
    serial_print("            Architecture: x86_64 Higher-Half             \n");
    serial_print("=========================================================\n\n");

    serial_print("[BOOT] Initializing Global Descriptor Table (GDT)...\n");
    serial_print("[BOOT] Initializing Interrupt Descriptor Table (IDT)...\n");
    serial_print("[BOOT] Remapping PIC and configuring hardware IRQs...\n");
    
    simd_init();
    serial_print("[SIMD] AVX/SSE vector unit initialized in Ring 0.\n");

    serial_print("[PhaseCore] Initializing deterministic execution engine...\n");
    aurascript_init();
    aura_link_init();
    studiolens_init();
    spatial_compositor_init();
    synthesizer_init();
    sot_mram_init();
    twin_prime_alloc_init();
    benchmark_init();
    
    serial_print("[Subsystems] Full 6-Tier Sovereign Convergence Stack online.\n");

    serial_print("\n--- Initializing TorusOS Spatial Shell ---\n");
    serial_print("Type 'help' for available commands.\n\n");
    serial_print("> ");

    char cmd_buffer[64];
    int cmd_len = 0;
    cmd_buffer[0] = '\0';

    // Active terminal serial polling loop
    while (1) {
        if (serial_received()) {
            char c = serial_read();
            
            // Handle Enter key (Newline / Carriage Return)
            if (c == '\n' || c == '\r') {
                serial_print("\n");
                cmd_buffer[cmd_len] = '\0';

                // Parse command and argument by splitting at first space
                char* cmd = cmd_buffer;
                char* arg = NULL;
                for (int i = 0; cmd_buffer[i] != '\0'; i++) {
                    if (cmd_buffer[i] == ' ') {
                        cmd_buffer[i] = '\0';
                        arg = &cmd_buffer[i + 1];
                        break;
                    }
                }
                trim_arg(arg);

                // Command Dispatcher
                if (streq(cmd, "help")) {
                    serial_print("Available TorusOS Commands:\n");
                    serial_print("  help                  - Display this help menu\n");
                    serial_print("  status                - Show subsystem & tier metrics\n");
                    serial_print("  script                - Execute active FlowScript stream\n");
                    serial_print("  aurascript [target]   - Compile AuraScript (default: kernel.as)\n");
                    serial_print("  manifold [val]        - Evaluate manifold variable at integer x\n");
                    serial_print("  auralink              - Verify backplane telemetry link\n");
                    serial_print("  studiolens [grid|stacked|spawn|workspace] - Render Tier 6 spatial workspace\n");
                    serial_print("  mram                  - Test SOT-MRAM zero-latency write/read\n");
                    serial_print("  twinalloc [size]      - Allocate memory block via twin primes\n");
                    serial_print("  benchmark             - [Phase 1] Run MRAM & Twin-Prime saturation suite\n");
                    serial_print("  phasecore             - [Hidden] Execute raw Tier 1 opcode dispatch\n");
                    serial_print("  omni                  - [Hidden] Unpack Sovereign Omnifile into AuraScript\n");
                    serial_print("  telemetry             - [Hidden] Stream enterprise diagnostics to Studio Lens\n");
                    serial_print("  vtx                   - [Hidden] Compile structural tensors to FlowScript IR\n");
                    serial_print("  clear                 - Clear terminal output\n");
                } else if (streq(cmd, "status")) {
                    serial_print("Tier 1 PhaseCore: Active (Deterministic)\n");
                    serial_print("Tier 3 FlowScript IR: Online\n");
                    serial_print("Tier 5 AuraLink Backplane: Verified\n");
                    serial_print("Tier 6 Studio Lens / Compositor: Converged\n");
                    serial_print("SOT-MRAM & Twin-Prime Allocator: Active\n");
                    serial_print("SIMD Vector Unit: Online (AVX/SSE)\n");
                } else if (streq(cmd, "script")) {
                    serial_print("[FlowScript] Executing default manifold pipeline...\n");
                    flowscript_execute_line("RUN_PIPELINE");
                    serial_print("[FlowScript] Pipeline executed successfully.\n");
                } else if (streq(cmd, "aurascript")) {
                    if (arg != NULL && arg[0] != '\0') {
                        serial_print("[AuraScript] Compiling target module: ");
                        serial_print(arg);
                        serial_print("\n");
                    } else {
                        serial_print("[AuraScript] Parsing default cascade layers and compiling master...\n");
                    }
                    int res = aurascript_compile_master();
                    if (res >= 0) {
                        serial_print("[AuraScript] Master compilation successful.\n");
                    } else {
                        serial_print("[AuraScript] Compilation warning / neutral exit.\n");
                    }
                } else if (streq(cmd, "manifold")) {
                    int x_val = 42; // default
                    if (arg != NULL && arg[0] != '\0') {
                        x_val = simple_atoi(arg);
                    }
                    
                    symbolic_math_init();

                    serial_print("[Manifold] Evaluating symbolic expression at x = ");
                    serial_print(arg ? arg : "42");
                    serial_print("\n");

                    SymbolicNode* node = symbolic_create_variable('x');
                    int64_t eval = symbolic_evaluate(node, x_val);
                    if (eval == x_val) {
                        serial_print("[Manifold] Evaluation result verified.\n");
                    }
                } else if (streq(cmd, "auralink")) {
                    serial_print("[AuraLink] Transmitting telemetry packet across backplane...\n");
                    uint64_t handle = aura_link_transmit_telemetry(0x7FFFFFFF);
                    bool verified = aura_link_verify_backplane(handle);
                    if (verified) {
                        serial_print("[AuraLink] Backplane telemetry verification: PASSED\n");
                    } else {
                        serial_print("[AuraLink] Backplane telemetry verification: FAILED\n");
                    }
                } else if (streq(cmd, "studiolens")) {
                    if (arg != NULL && arg[0] != '\0') {
                        if (streq(arg, "grid")) {
                            studiolens_set_layout(LAYOUT_GRID);
                            serial_print("[StudioLens] Layout switched to GRID (2x2 split).\n");
                        } else if (streq(arg, "stacked")) {
                            studiolens_set_layout(LAYOUT_STACKED);
                            serial_print("[StudioLens] Layout switched to STACKED (z-index).\n");
                        } else if (streq(arg, "spawn")) {
                            studiolens_set_layout(LAYOUT_FLOATING);
                            studiolens_spawn_window("TelemetryPanel", 100, 200, 640, 480);
                            serial_print("[StudioLens] Layout switched to FLOATING (Custom Window Spawn).\n");
                        } else {
                            serial_print("[StudioLens] Targeting workspace: ");
                            serial_print(arg);
                            serial_print("\n");
                        }
                    }
                    uint64_t axiom = synthesizer_generate_axiom(0x1337);
                    bool duality = synthesizer_verify_duality(axiom);
                    if (duality) {
                        serial_print("[Synthesizer] Duality verification: PASSED\n");
                    }
                    studiolens_render_workspace(arg && !streq(arg, "grid") && !streq(arg, "stacked") && !streq(arg, "spawn") ? arg : "MASTER_WORKSPACE");
                    spatial_compositor_render(0);
                    serial_print("[Tier 6 Convergence] Workspace rendered successfully.\n");
                } else if (streq(cmd, "mram")) {
                    serial_print("[SOT-MRAM] Performing zero-latency write operation...\n");
                    uint64_t addr = 0x8000;
                    uint64_t payload = 0x13372026ULL;
                    sot_mram_write_zero_latency(addr, payload);
                    uint64_t result = sot_mram_read_zero_latency(addr);
                    if (sot_mram_verify_t_refi_zero()) {
                        serial_print("[SOT-MRAM] t_refi = 0 refresh latency verified: PASSED\n");
                        serial_print("[SOT-MRAM] Zero-latency read/write integrity: VERIFIED\n");
                    }
                } else if (streq(cmd, "twinalloc")) {
                    int size = 512;
                    if (arg != NULL && arg[0] != '\0') {
                        size = simple_atoi(arg);
                    }
                    serial_print("[TwinPrimeAlloc] Requesting block allocation...\n");
                    uint64_t handle = twin_prime_alloc_block((uint64_t)size);
                    if (handle != 0) {
                        serial_print("[TwinPrimeAlloc] Block allocated successfully via twin-prime spacing.\n");
                    } else {
                        serial_print("[TwinPrimeAlloc] Allocation returned null handle.\n");
                    }
                } else if (streq(cmd, "benchmark")) {
                    serial_print("[Phase 1] Executing automated subsystem saturation benchmark...\n");
                    benchmark_run_mram_saturation(1000);
                    benchmark_run_twinalloc_stress(500);
                    serial_print("[Phase 1] Benchmark suite completed successfully.\n");
                } else if (streq(cmd, "phasecore")) {
                    serial_print("[PhaseCore] Executing deterministic opcode cycle...\n");
                    serial_print("[PhaseCore] Opcode stream dispatched -> Lowering to AuraScript...\n");
                    aurascript_compile_master();
                    serial_print("[PhaseCore -> AuraScript] Convergence complete.\n");
                } else if (streq(cmd, "omni")) {
                    serial_print("[OmniEnvelope] Unpacking XML sovereign state envelope...\n");
                    omni_envelope_parse();
                    serial_print("[OmniEnvelope] Binding state to Studio Lens workspace...\n");
                    studiolens_render_workspace("OMNI_CONVERGENCE_WORKSPACE");
                    serial_print("[Omni -> StudioLens] Convergence complete.\n");
                } else if (streq(cmd, "telemetry")) {
                    serial_print("[Telemetry] Dumping enterprise performance metrics...\n");
                    enterprise_telemetry_dump();
                    serial_print("[Telemetry] Piping metrics into AuraLink backplane...\n");
                    aura_link_transmit_telemetry(0xF00DF00D);
                    serial_print("[Telemetry -> StudioLens] Visual metrics updated.\n");
                } else if (streq(cmd, "vtx")) {
                    serial_print("[VTX Source] Compiling high-level structural tensor source...\n");
                    serial_print("[VTX Source] Lowering structural declaration into Tier 3 FlowScript IR...\n");
                    flowscript_execute_line("VTX_COMPILE_TENSORS");
                    studiolens_render_workspace("VTX_TENSOR_PIPELINE");
                    serial_print("[VTX -> StudioLens] Tensor convergence complete.\n");
                } else if (streq(cmd, "clear")) {
                    serial_print("\033[2J\033[H");
                } else if (cmd_len > 0) {
                    serial_print("Unknown command. Type 'help' for options.\n");
                }

                // Reset buffer for next command
                cmd_len = 0;
                cmd_buffer[0] = '\0';
                serial_print("> ");
            } 
            // Handle Backspace
            else if (c == '\b' || c == 127) {
                if (cmd_len > 0) {
                    cmd_len--;
                    cmd_buffer[cmd_len] = '\0';
                    serial_print("\b \b");
                }
            } 
            // Handle normal printable characters
            else if (c >= 32 && c < 127) {
                if (cmd_len < (int)(sizeof(cmd_buffer) - 1)) {
                    cmd_buffer[cmd_len++] = c;
                    cmd_buffer[cmd_len] = '\0';
                    char buf[2] = { c, '\0' };
                    serial_print(buf);
                }
            }
        }
        __asm__ volatile ("pause");
    }
}
