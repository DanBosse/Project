with open("cli.c", "r") as f:
    code = f.read()

# Make sure studiolens_dashboard.h is included
if '#include "studiolens_dashboard.h"' not in code:
    code = '#include "studiolens_dashboard.h"\n' + code

target = """    if (k_strcmp(cmd, "help") == 0) {
        cli_cmd_help();
    }"""

replacement = """    if (k_strcmp(cmd, "help") == 0) {
        cli_cmd_help();
    }
    else if (k_strcmp(cmd, "dashboard") == 0) {
        studiolens_render_telemetry_dashboard();
    }"""

if 'k_strcmp(cmd, "dashboard")' not in code:
    code = code.replace(target, replacement)
    with open("cli.c", "w") as f:
        f.write(code)
    print("Successfully patched cli.c to handle 'dashboard' command!")
else:
    print("'dashboard' already handled in cli.c")
