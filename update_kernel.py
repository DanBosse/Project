with open("kernel.c", "r") as f:
    kc = f.read()

# Make sure command_registry.h is included
if "#include \"command_registry.h\"" not in kc:
    kc = "#include \"command_registry.h\"\n" + kc

# Update the help case in kernel_main to use shell_print_help(arg)
# Look for streq(cmd, "help") block and replace its body
import re
old_help_pattern = r'else if \(streq\(cmd, "help"\)\)\s*\{[^}]*\}'

new_help_block = '''else if (streq(cmd, "help")) {
                    // Extract argument if present (e.g., "help telemetry")
                    char *arg = cmd + 4;
                    while (*arg == ' ') arg++;
                    shell_print_help(arg);
                }'''

if re.search(old_help_pattern, kc):
    kc = re.sub(old_help_pattern, new_help_block, kc, count=1)
    with open("kernel.c", "w") as f:
        f.write(kc)
    print("Successfully wired shell_print_help into kernel.c!")
else:
    print("Could not find exact help command block in kernel.c, please check manually.")
