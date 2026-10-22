# Helper to patch torus.c shell to recognize 'flowscript'
with open("torus.c", "r") as f:
    code = f.read()

if "flowscript_execute" not in code:
    # Insert initialization and command parsing
    code = code.replace(
        '#include "torus.h"',
        '#include "torus.h"\n#include "flowscript.h"'
    )
    # Add command handling inside shell loop
    target = 'else if (streq(cmd, "help")) {'
    replacement = '''else if (streq(cmd, "flowscript")) {
            serial_print("Usage: flowscript <sync_all | pulse_manifold | script_name>\\n");
        } else if (streq(cmd, "fs_sync")) {
            flowscript_execute("sync_all");
        } else if (streq(cmd, "fs_pulse")) {
            flowscript_execute("pulse_manifold");
        } ''' + target
    code = code.replace(target, replacement)

    with open("torus.c", "w") as f:
        f.write(code)
    print("Successfully patched torus.c with FlowScript commands!")
else:
    print("torus.c already includes FlowScript bindings.")
