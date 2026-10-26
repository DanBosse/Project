with open('command_registry.c', 'r', encoding='utf-8') as f:
    content = f.read()

if '#include "net_stack.h"' not in content:
    # Insert right after other local headers
    target = '#include "studio_core.h"'
    if target in content:
        content = content.replace(target, target + '\n#include "net_stack.h"')
    else:
        content = '#include "net_stack.h"\n' + content

with open('command_registry.c', 'w', encoding='utf-8') as f:
    f.write(content)

print("[+] Added net_stack.h include to command_registry.c")
