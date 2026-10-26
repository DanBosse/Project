import os

# 1. Fix aurascript.c by adding #include "vfs.h" if not present
aurascript_path = "aurascript.c"
if os.path.exists(aurascript_path):
    with open(aurascript_path, "r") as f:
        content = f.read()
    if '#include "vfs.h"' not in content:
        # Insert after the first include or at the top
        include_target = '#include "aurascript.h"'
        if include_target in content:
            content = content.replace(include_target, f'{include_target}\n#include "vfs.h"')
        else:
            content = '#include "vfs.h"\n' + content
        with open(aurascript_path, "w") as f:
            f.write(content)
        print("[+] Added #include \"vfs.h\" to aurascript.c")

# 2. Fix command_registry.c function declarations/signatures
registry_path = "command_registry.c"
if os.path.exists(registry_path):
    with open(registry_path, "r") as f:
        code = f.read()
    
    # Replace the conflicting static wrappers
    old_comms = 'static void cmd_officecomms(const char *arg) { cmd_office_comms(arg); }'
    new_comms = 'void cmd_officecomms(void) { cmd_office_comms(); }'
    
    old_telem = 'static void cmd_officetelemetry(void) { cmd_office_telemetry(); }'
    new_telem = 'void cmd_officetelemetry(void) { cmd_office_telemetry(); }'
    
    if old_comms in code:
        code = code.replace(old_comms, new_comms)
    if old_telem in code:
        code = code.replace(old_telem, new_telem)
        
    with open(registry_path, "w") as f:
        f.write(code)
    print("[+] Fixed office command signatures in command_registry.c")

print("Patch applied successfully!")
