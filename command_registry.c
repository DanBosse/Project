
extern void cli_cmd_aura(const char *arg);
extern void cli_cmd_firewall(const char *arg);
extern void cli_cmd_np_test(const char *arg);
extern void cli_cmd_np_bench(const char *arg);
#include "command_registry.h"
#include <stddef.h>

void shell_cmd_clear(const char *arg) {
    (void)arg;
}

static void cmd_help_text(const char *arg) {
    (void)arg;
}

extern void cli_cmd_aura(const char *arg);
extern void firewall_cli_handler(const char *arg);
extern void np_test_cli_handler(const char *arg);

static const __attribute__((unused)) command_t internal_commands[] = {
    { "aura", "", "Interface with the Aura Subsystem", "system", (cmd_handler_t)cli_cmd_aura, false },
    { "firewall", "", "Display TensorShield & NP-Handshake firewall status", "security", (cmd_handler_t)firewall_cli_handler, false },
    { "np_test", "", "Simulate and test NP-complete handshake puzzle response", "security", (cmd_handler_t)np_test_cli_handler, false },
    { "help", "", "Display system information and available commands", "system", (cmd_handler_t)cmd_help_text, false }
};

bool shell_dispatch_command(const char *cmd, const char *arg) {
    (void)cmd;
    (void)arg;
    return true;
}
