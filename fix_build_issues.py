# 1. Fix physics_studio.c includes
with open("physics_studio.c", "r", encoding="utf-8") as f:
    code = f.read()

includes = """#include "physics_studio.h"
#include "graphics_studio.h"
#include "audio_studio.h"
#include "video_studio.h"
#include "auracad.h"
#include "office_suite.h"
#include "aura_link.h"
#include "studiolens.h"
"""

if '#include "graphics_studio.h"' not in code:
    code = includes + code
    with open("physics_studio.c", "w", encoding="utf-8") as f:
        f.write(code)
    print("Added missing header includes to physics_studio.c")

# 2. Fix command_registry.c strcmp usage
with open("command_registry.c", "r", encoding="utf-8") as f:
    creg = f.read()

creg = creg.replace('strcmp(', '__builtin_strcmp(')

with open("command_registry.c", "w", encoding="utf-8") as f:
    f.write(creg)
print("Updated strcmp to __builtin_strcmp in command_registry.c")
