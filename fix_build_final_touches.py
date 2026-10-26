import os

# 1. Add ssize_t definition to stubs.c if missing
if os.path.exists("stubs.c"):
    with open("stubs.c", "r") as f:
        content = f.read()
    if "ssize_t" in content and "ssize_t" not in content[:content.find("ssize_t")]: # check if typedef exists
        # Prepend definition
        header_def = "#ifndef _SSIZE_T_DEFINED\n#define _SSIZE_T_DEFINED\ntypedef long ssize_t;\n#endif\n\n"
        if "_SSIZE_T_DEFINED" not in content:
            content = header_def + content
            with open("stubs.c", "w") as f:
                f.write(content)
            print("[+] Added ssize_t definition to stubs.c")

# 2. Add #include "office_suite.h" to physics_studio.c
if os.path.exists("physics_studio.c"):
    with open("physics_studio.c", "r") as f:
        p_content = f.read()
    if '#include "office_suite.h"' not in p_content:
        p_content = '#include "office_suite.h"\n' + p_content
        with open("physics_studio.c", "w") as f:
            f.write(p_content)
        print("[+] Added #include \"office_suite.h\" to physics_studio.c")

print("Fixes applied!")
