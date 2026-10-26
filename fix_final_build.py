import os

# 1. Fix command_registry.c to match office_suite.h (accept const char *arg and pass it)
registry_path = "command_registry.c"
if os.path.exists(registry_path):
    with open(registry_path, "r") as f:
        code = f.read()
    
    # Replace the void wrappers with arg-accepting wrappers
    old_comms = 'void cmd_officecomms(void) { cmd_office_comms(); }'
    new_comms = 'void cmd_officecomms(const char *arg) { cmd_office_comms(arg); }'
    
    old_telem = 'void cmd_officetelemetry(void) { cmd_officetelemetry(); }'
    new_telem = 'void cmd_officetelemetry(const char *arg) { cmd_office_telemetry(arg); }'
    
    # Also check variant with static if any
    code = code.replace('static void cmd_officecomms(void)', 'void cmd_officecomms(const char *arg)')
    code = code.replace('static void cmd_officetelemetry(void)', 'void cmd_officetelemetry(const char *arg)')
    
    code = code.replace('cmd_office_comms();', 'cmd_office_comms(arg);')
    code = code.replace('cmd_office_telemetry();', 'cmd_office_telemetry(arg);')
    
    if old_comms in code:
        code = code.replace(old_comms, new_comms)
    if old_telem in code:
        code = code.replace(old_telem, new_telem)
        
    with open(registry_path, "w") as f:
        f.write(code)
    print("[+] Updated command_registry.c function wrappers to accept arg")

# 2. Ensure aurascript.c includes vfs.h and has proper prototypes if needed
aurascript_path = "aurascript.c"
if os.path.exists(aurascript_path):
    with open(aurascript_path, "r") as f:
        content = f.read()
    if '#include "vfs.h"' not in content:
        content = '#include "vfs.h"\n' + content
    
    # Add explicit prototypes just in case vfs.h lacks them
    proto = '\nint vfs_open(const char *path, int flags);\nssize_t vfs_read(int fd, void *buf, size_t count);\nint vfs_close(int fd);\n'
    if 'vfs_open' not in content:
        content += proto
        
    with open(aurascript_path, "w") as f:
        f.write(content)
    print("[+] Updated aurascript.c with VFS header and prototypes")

print("Patch applied successfully!")
