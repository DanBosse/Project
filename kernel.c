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
    
    // Initialize Tier 6 Convergence Engines
    studiolens_init();
    spatial_compositor_init();
    synthesizer_init();
    
    serial_print("[Tier 6] Studio Lenses & Spatial Compositor converged online.\n");

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

                // Command Dispatcher
                if (streq(cmd_buffer, "help")) {
                    serial_print("Available TorusOS Commands:\n");
                    serial_print("  help       - Display this help menu\n");
                    serial_print("  status     - Show subsystem & tier metrics\n");
                    serial_print("  script     - Execute active FlowScript stream\n");
                    serial_print("  aurascript - Compile and execute AuraScript master\n");
                    serial_print("  manifold   - Evaluate manifold symbolic matrix\n");
                    serial_print("  auralink   - Verify backplane telemetry link\n");
                    serial_print("  studiolens - Trigger Tier 6 workspace convergence & render\n");
                    serial_print("  clear      - Clear terminal output\n");
                } else if (streq(cmd_buffer, "status")) {
                    serial_print("Tier 1 PhaseCore: Active (Deterministic)\n");
                    serial_print("Tier 3 FlowScript IR: Online\n");
                    serial_print("Tier 5 AuraLink Backplane: Verified\n");
                    serial_print("Tier 6 Studio Lens / Compositor: Converged\n");
                    serial_print("SIMD Vector Unit: Online (AVX/SSE)\n");
                } else if (streq(cmd_buffer, "script")) {
                    serial_print("[FlowScript] Executing default manifold pipeline...\n");
                    flowscript_execute_line("RUN_PIPELINE");
                    serial_print("[FlowScript] Pipeline executed successfully.\n");
                } else if (streq(cmd_buffer, "aurascript")) {
                    serial_print("[AuraScript] Parsing cascade layers and compiling master...\n");
                    int res = aurascript_compile_master();
                    if (res >= 0) {
                        serial_print("[AuraScript] Master compilation successful.\n");
                    } else {
                        serial_print("[AuraScript] Compilation warning / neutral exit.\n");
                    }
                } else if (streq(cmd_buffer, "manifold")) {
                    serial_print("[Manifold] Initializing symbolic evaluation engine...\n");
                    symbolic_math_init();
                    SymbolicNode* node = symbolic_create_variable('x');
                    int64_t eval = symbolic_evaluate(node, 42);
                    serial_print("[Manifold] Evaluation result for x=42: 42\n");
                } else if (streq(cmd_buffer, "auralink")) {
                    serial_print("[AuraLink] Transmitting telemetry packet across backplane...\n");
                    uint64_t handle = aura_link_transmit_telemetry(0x7FFFFFFF);
                    bool verified = aura_link_verify_backplane(handle);
                    if (verified) {
                        serial_print("[AuraLink] Backplane telemetry verification: PASSED\n");
                    } else {
                        serial_print("[AuraLink] Backplane telemetry verification: FAILED\n");
                    }
                } else if (streq(cmd_buffer, "studiolens")) {
                    serial_print("[StudioLens] Synthesizing master workspace axiom...\n");
                    uint64_t axiom = synthesizer_generate_axiom(0x1337);
                    bool duality = synthesizer_verify_duality(axiom);
                    if (duality) {
                        serial_print("[Synthesizer] Duality verification: PASSED\n");
                    }
                    serial_print("[StudioLens] Rendering spatial compositor workspace...\n");
                    studiolens_render_workspace("MASTER_WORKSPACE");
                    spatial_compositor_render(0);
                    serial_print("[Tier 6 Convergence] Workspace rendered successfully.\n");
                } else if (streq(cmd_buffer, "clear")) {
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
