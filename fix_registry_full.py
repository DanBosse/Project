import re
lines = [
"#include <stdbool.h>",
"#include <stddef.h>",
"void serial_print(const char *s);",
"void shell_cmd_clear(const char *arg) { serial_print(\"\\033[2J\\033[H\"); }",
"void shell_print_help(const char *arg);",
"typedef void (*cmd_handler_t)(const char *);",
"typedef struct { const char *name; const char *description; const char *category; cmd_handler_t handler; bool system; } command_t;"
]
lines.extend([
"static command_t global_commands[] = {",
"    { \"help\", \"Display this help menu\", \"system\", shell_print_help, false },",
"    { \"clear\", \"Clear terminal output\", \"system\", shell_cmd_clear, false },",
"    { \"studio\", \"Launch StudioLens Spatial Environment\", \"studio\", NULL, false }",
"};",
"static int cmd_registry_count = 3;"
])
lines.extend([
"void shell_print_help(const char *arg) {",
"    char cat_buf[32] = {0}; int idx = 0;",
"    if (arg) {",
"        while (*arg == ' ' || *arg == '\"' || *arg == '\\'') arg++;",
"        while (arg[idx] != '\\0' && idx < 31) {",
"            if (arg[idx] == ' ' || arg[idx] == '\"' || arg[idx] == '\\'') break;",
"            cat_buf[idx] = arg[idx]; idx++;",
"        }",
"    }",
"    serial_print(\"[TorusOS Help System]\\n\");",
"    for (int i = 0; i < cmd_registry_count; i++) {",
"        if (cat_buf[0] != '\\0' && re_match(cat_buf, global_commands[i].category)) continue;",
"        serial_print(\"  \"); serial_print(global_commands[i].name);",
"        serial_print(\" - \"); serial_print(global_commands[i].description); serial_print(\"\\n\");",
"    }",
"}",
"bool shell_dispatch_command(const char *cmd) {",
"    if (!cmd || *cmd == '\\0') return false;",
"    for (int i = 0; i < cmd_registry_count; i++) {",
"        int j = 0; while (cmd[j] != '\\0' && cmd[j] != ' ' && global_commands[i].name[j] != '\\0') {",
"            if (cmd[j] != global_commands[i].name[j]) break; j++;",
"        }",
"        if ((cmd[j] == '\\0' || cmd[j] == ' ') && global_commands[i].name[j] == '\\0') {",
"            if (global_commands[i].handler) { global_commands[i].handler(cmd + j); return true; }",
"            serial_print(\"Command found but handler is stubbed.\\n\"); return true;",
"        }",
"    }",
"    serial_print(\"Unknown TorusOS command.\\n\"); return false;",
"}"
])
with open("command_registry.c", "w") as f: f.write("\n".join(lines) + "\n")
print("Reassembled command_registry.c with active dispatch hooks.")
with open("command_registry.c", "w") as f: f.write("\n".join(lines) + "\n")
print("Reassembled command_registry.c with active dispatch hooks.")
