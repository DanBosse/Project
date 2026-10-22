import re

# 1. Update command_registry.h to include execution dispatch
registry_h = """#ifndef COMMAND_REGISTRY_H
#define COMMAND_REGISTRY_H

#include <stdbool.h>

typedef void (*cmd_handler_t)(const char *arg);

typedef struct {
    const char *name;
    const char *args;
    const char *description;
    const char *category;
    cmd_handler_t handler;
    bool hidden;
} command_t;

void shell_print_help(const char *category);
bool shell_dispatch_command(const char *cmd, const char *arg);

#endif
"""

with open("command_registry.h", "w") as f:
    f.write(registry_h)

print("Updated command_registry.h with dispatch signature!")
