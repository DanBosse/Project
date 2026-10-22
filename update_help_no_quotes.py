with open("command_registry.c", "r") as f:
    code = f.read()

# Update shell_print_help to clean quotes and handle unquoted category strings cleanly
new_help_func = """void shell_print_help(const char *arg) {
    // Strip surrounding quotes or whitespace if present
    char cat_buf[32];
    int idx = 0;
    if (arg) {
        while (*arg == ' ' || *arg == '\\"' || *arg == '\\'') arg++;
        while (arg[idx] != '\\0' && idx < 31) {
            if (arg[idx] == '\\"' || arg[idx] == '\\'' || arg[idx] == '\\n' || arg[idx] == '\\r') break;
            cat_buf[idx] = arg[idx];
            idx++;
        }
        cat_buf[idx] = '\\0';
        // trim trailing spaces
        while (idx > 0 && cat_buf[idx - 1] == ' ') {
            cat_buf[--idx] = '\\0';
        }
    } else {
        cat_buf[0] = '\\0';
    }

    serial_print("[TorusOS Spatial Shell] === Command Registry & Help ===\\n");
    if (cat_buf[0] != '\\0') {
        serial_print("Filtered by category: ");
        serial_print(cat_buf);
        serial_print("\\n---------------------------------------------------------\\n");
    }

    for (size_t i = 0; i < sizeof(global_commands) / sizeof(command_t); i++) {
        if (global_commands[i].hidden) continue;

        if (cat_buf[0] != '\\0') {
            const char *c = global_commands[i].category;
            bool match = true;
            for (int j = 0; cat_buf[j] != '\\0' && c[j] != '\\0'; j++) {
                if (cat_buf[j] != c[j]) { match = false; break; }
            }
            if (cat_buf[j] != '\\0' || c[j] != '\\0') match = false;
            if (!match) continue;
        }

        serial_print("  ");
        serial_print(global_commands[i].name);
        serial_print(" ");
        serial_print(global_commands[i].args);
        serial_print(" - ");
        serial_print(global_commands[i].description);
        serial_print("\\n");
    }
    serial_print("=========================================================\\n");
    serial_print("Tip: Use 'help telemetry' or 'help pipeline' for category views\\n");
}"""

# Replace old shell_print_help in command_registry.c
import re
code = re.sub(r"void shell_print_help\(const char \*category\) \{.*?\^\}", new_help_func, code, flags=re.DOTALL)

with open("command_registry.c", "w") as f:
    f.write(code)

print("Updated command_registry.c for unquoted category filtering.")
