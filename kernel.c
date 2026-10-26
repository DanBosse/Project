#include <stdint.h>
#include "aurascript.h"

struct idt_entry {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t ist;
    uint8_t flags;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

static struct idt_entry idt[256];
static struct idt_ptr idtp;

void idt_set_gate(uint8_t vector, uint64_t handler, uint16_t selector, uint8_t flags) {
    idt[vector].offset_low = (uint16_t)(handler & 0xFFFF);
    idt[vector].selector = selector;
    idt[vector].ist = 0;
    idt[vector].flags = flags;
    idt[vector].offset_middle = (uint16_t)((handler >> 16) & 0xFFFF);
    idt[vector].offset_high = (uint32_t)(handler >> 32);
    idt[vector].reserved = 0;
}

#include "command_registry.h"
#include "aurascript.h"
#include "physics_studio.h"
#include "aurascript.h"
#include "studiolens_dashboard.h"
#include "aurascript.h"
#include "aura.h"
#include "aurascript.h"
#include "auracad.h"
#include "aurascript.h"
#include "video_studio.h"
#include "aurascript.h"
#include "audio_studio.h"
#include "aurascript.h"
#include "graphics_studio.h"
#include "aurascript.h"
#include "office_suite.h"
#include "aurascript.h"
#include "physics_lab.h"
#include "aurascript.h"
#include <stdint.h>
#include "aurascript.h"
#include <stddef.h>
#include "aurascript.h"
#include "torus.h"
#include "aurascript.h"
#include "dpu.h"
#include "aurascript.h"
#include "vpu.h"
#include "aurascript.h"
#include "coprocessor.h"
#include "aurascript.h"
#include "phasecore.h"
#include "aurascript.h"
#include "simd.h"
#include "aurascript.h"
#include "flowscript.h"
#include "aurascript.h"
#include "aurascript_compiler.h"
#include "aurascript.h"
#include "aura_link.h"
#include "aurascript.h"
#include "symbolic_math.h"
#include "aurascript.h"
#include "studiolens.h"
#include "aurascript.h"
#include "spatial_compositor.h"
#include "aurascript.h"
#include "synthesizer.h"
#include "aurascript.h"
#include "sot_mram.h"
#include "aurascript.h"
#include "twin_prime_alloc.h"
#include "aurascript.h"
#include "omni_envelope.h"
#include "aurascript.h"
#include "enterprise_telemetry.h"
#include "aurascript.h"
#include "benchmark.h"
#include "aurascript.h"

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

// Simple prefix check helper
int strstarts(const char* pre, const char* str) {
    size_t lenpre = 0;
    while (pre[lenpre] != '\0') lenpre++;
    size_t lenstr = 0;
    while (str[lenstr] != '\0') lenstr++;
    if (lenpre > lenstr) return 0;
    for (size_t i = 0; i < lenpre; i++) {
        if (str[i] != pre[i]) return 0;
    }
    return 1;
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
    extern void isr13_wrapper(void);
extern void isr0x80_wrapper(void);
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint64_t)&idt;
    idt_set_gate(13, (uint64_t)isr13_wrapper, 0x08, 0x8E);
    idt_set_gate(0x80, (uint64_t)isr0x80_wrapper, 0x08, 0xEE);
    __asm__ volatile("lidt %0" : : "m"(idtp));
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

    char cmd_buffer[128];
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

                // Check for tripartite search commands (supporting prefix match)
                if (strstarts("search:data", cmd_buffer) || 
                    strstarts("search:net", cmd_buffer) || 
                    strstarts("search:os", cmd_buffer)) {
                    studiolens_execute_command(cmd_buffer);
                } else {
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
                // Left-trim leading spaces from cmd
                while (*cmd == ' ' || *cmd == '\t') {
                    cmd++;
                }
                if (cmd_len > 0) {
                    if (!shell_dispatch_command(cmd, arg)) {
                        serial_print("Unknown command. Type 'help' for options.\n");
                    }
                }
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
