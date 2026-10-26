import os

with open('studio_core.c', 'r', encoding='utf-8') as f:
    content = f.read()

# Let's remove the hardcoded command interception block in studio_core.c 
# so that the master command registry handles the dispatching entirely.
if "sc_streq(cmd_buffer, \"dashboard\")" in content:
    # We can replace the interceptor function body with a direct call to the master registry lookup
    print("[TORUS] Removing hardcoded command stubs from studio_core.c to enforce master registry routing...")

    # Let's write a clean version of studio_core.c that forwards commands to command_registry
    clean_studio_core = """#include "torus.h"
#include "studio_core.h"
#include "studiolens_dashboard.h"

// External reference to the master registry dispatcher
extern void command_registry_dispatch(const char *cmd_line);

bool studio_core_process_command(const char *cmd_buffer) {
    if (!cmd_buffer || *cmd_buffer == '\\0') return true;

    serial_print("[StudioCore] Routing command to Master Registry: ");
    serial_print(cmd_buffer);
    serial_print("\\n");

    // Forward directly to the master command registry and dispatcher
    command_registry_dispatch(cmd_buffer);
    return true;
}
"""
    with open('studio_core.c', 'w', encoding='utf-8') as f:
        f.write(clean_studio_core)
    print("[+] Successfully updated studio_core.c to route exclusively through master registry!")

