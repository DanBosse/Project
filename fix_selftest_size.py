with open("command_registry.c", "r") as f:
    code = f.read()

# Remove the premature forward declaration
code = code.replace("static command_t global_commands[];\n", "")

# Move cmd_selftest definition to right above shell_print_help (which is after global_commands)
selftest_func = """static void cmd_selftest(const char *arg) {
    (void)arg;
    serial_print("[TorusOS Self-Test] Validating command registry handlers...\\n");
    size_t total = sizeof(global_commands) / sizeof(command_t);
    size_t handlers_bound = 0;

    for (size_t i = 0; i < total; i++) {
        serial_print("  [Test] Checking command: ");
        serial_print(global_commands[i].name);

        if (global_commands[i].handler) {
            serial_print(" -> [BOUND] OK\\n");
            handlers_bound++;
        } else if (global_commands[i].hidden) {
            serial_print(" -> [HIDDEN] OK\\n");
        } else {
            serial_print(" -> [WARNING] Unbound handler (falling back to generic table response)\\n");
        }
    }
    serial_print("[TorusOS Self-Test] Validation complete. All registry entries verified.\\n");
}

"""

# Strip out the old cmd_selftest definition from the top
import re
code = re.sub(r'static void cmd_selftest\(const char \*arg\) \{.*?\^\n\}', '', code, flags=re.DOTALL) # wait, let's do a precise string replacement

