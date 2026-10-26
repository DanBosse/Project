
#include "vfs.h"
#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef long ssize_t;
#endif

#include "aurascript.h"
#include <stdint.h>
#include <stdbool.h>

extern void serial_print(const char* str);
extern void shell_execute_command(const char* cmd);

static int var_omega_mass = 10;
static int var_omega_damping = 5;
static int var_viewport_scale = 100;
static bool ipc_channel_active = true;

void aurascript_init(void) {
    serial_print("[AuraScript] Initializing Core Multi-Paradigm Engine & IPC Channels...\n");
}

void aurascript_dump_full_source(void) {
    serial_print("[AuraScript] Dumping UserTerminal Node Source Specification:\n");
    serial_print("node UserTerminal {\n");
    serial_print("    layout: vertical_stack;\n");
    serial_print("    prompt: \"user@torus-sandbox:/# \";\n");
    serial_print("    allow_ipc: true;\n");
    serial_print("}\n");
}

bool aurascript_ipc_transmit(const char *data) {
    if (!ipc_channel_active || !data) return false;
    serial_print("[IPC] Transmitting data packet through spatial viewport backplane...\n");
    return true;
}

void aurascript_eval(const char *expr) {
    if (!expr) return;
    serial_print("[UserTerminal] Input received -> Routing to execution stack: ");
    serial_print(expr);
    serial_print("\n");
    
    if (ipc_channel_active) {
        aurascript_ipc_transmit(expr);
    }
}

void aurascript_set_variable(const char *name, int value) {
    if (name) {
        if (name[0] == 'o') {
            var_omega_mass = value;
        } else if (name[0] == 'v') {
            var_viewport_scale = value;
        }
    }
}


#include "vfs.h"
#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef long ssize_t;
#endif

void aurascript_launch_ring3_container(void) {
    serial_print("[AuraScript] Loading container specification from user_terminal.as...\n");
    int fd = vfs_open("/user_terminal.as", 0);
    if (fd >= 0) {
        char buffer[512];
        ssize_t bytes_read = vfs_read(fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            serial_print("[AuraScript] Successfully parsed user_terminal.as container specification.\n");
        }
        vfs_close(fd);
    } else {
        serial_print("[AuraScript] Warning: user_terminal.as not found, utilizing embedded Ring3 profile.\n");
    }
}
