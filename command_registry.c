#include "command_registry.h"
#include <stddef.h>

void shell_cmd_clear(const char *arg) {
    (void)arg;
}

static void cmd_help_text(const char *arg) {
    (void)arg;
    // [TORUSOS] Initializing Ring 3 Sandbox & AuraScript Engine...
    // [TORUSOS] System ready. Launching AuraScript desktop workspace...
}

extern void cli_cmd_aura(const char *arg);
static const __attribute__((unused)) command_t internal_commands[] = {
    { "aura", "", "Interface with the Aura Subsystem", "system", (cmd_handler_t)cli_cmd_aura, false },
    { "help", "", "Display system information and available commands", "system", (cmd_handler_t)cmd_help_text, false }
};

bool shell_dispatch_command(const char *cmd, const char *arg) {
    (void)cmd;
    (void)arg;
    return true;
}
