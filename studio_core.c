#include <stdbool.h>
#include <string.h>
#include <stddef.h>

extern bool shell_dispatch_command(const char *cmd, const char *arg);
extern void studiolens_render_telemetry_dashboard(void);

void studio_core_init(void) {
    // Initialize studio core subsystems
}

size_t studio_core_read_pipe(void *buf, size_t len) {
    // Stub pipe reader for audio/aura subsystems
    return 0;
}

void shell_dispatch_command_line(const char *cmd_line) {
    char buf[128];
    int i = 0;
    while (cmd_line[i] && cmd_line[i] != ' ' && i < 127) {
        buf[i] = cmd_line[i];
        i++;
    }
    buf[i] = '\0';
    const char *arg = "";
    if (cmd_line[i] == ' ') {
        arg = &cmd_line[i + 1];
    }
    shell_dispatch_command(buf, arg);
}

void studio_core_dispatch(void) {
    studiolens_render_telemetry_dashboard();
}
