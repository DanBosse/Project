import re

# 1. Fix gpf.c to include kernel.h
with open("gpf.c", "r", encoding="utf-8") as f:
    gpf_content = f.read()

if "#include \"kernel.h\"" not in gpf_content:
    gpf_content = '#include "kernel.h"\n' + gpf_content
    with open("gpf.c", "w", encoding="utf-8") as f:
        f.write(gpf_content)
    print("Fixed gpf.c header inclusion.")

# kernel.c restructuring disabled to protect string literals
