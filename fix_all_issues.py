import os

# 1. Fix aurascript.c by adding #include "vfs.h"
aurascript_path = "aurascript.c"
if os.path.exists(aurascript_path):
    with open(aurascript_path, "r") as f:
        content = f.read()
    if '#include "vfs.h"' not in content:
        content = '#include "vfs.h"\n' + content
        with open(aurascript_path, "w") as f:
            f.write(content)
        print("[+] Added #include \"vfs.h\" to aurascript.c")

# 2. Fix office_suite.h to match office_suite.c signatures (taking const char *arg)
header_path = "office_suite.h"
if os.path.exists(header_path):
    with open(header_path, "r") as f:
        h_content = f.read()
    
    h_content = h_content.replace('void cmd_office_comms(void);', 'void cmd_office_comms(const char *arg);')
    h_content = h_content.replace('void cmd_officetelemetry(void);', 'void cmd_officetelemetry(const char *arg);')
    
    with open(header_path, "w") as f:
        f.write(h_content)
    print("[+] Updated office_suite.h signatures to match office_suite.c")

print("All fixes applied!")
