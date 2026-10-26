import os

with open('studio_core.c', 'r', encoding='utf-8') as f:
    content = f.read()

# Ensure studiolens_dashboard.h is included
if '#include "studiolens_dashboard.h"' not in content:
    content = '#include "studiolens_dashboard.h"\n' + content

# Replace the dashboard stub block with the actual function call
old_block = """    if (sc_streq(cmd_buffer, "dashboard") == 0) {
        serial_print("[StudioCore] Routing to StudioLens Telemetry Dashboard...\\n");
        return true;
    }"""

new_block = """    if (sc_streq(cmd_buffer, "dashboard") == 0) {
        serial_print("[StudioCore] Routing to StudioLens Telemetry Dashboard...\\n");
        studiolens_render_telemetry_dashboard();
        return true;
    }"""

if old_block in content:
    content = content.replace(old_block, new_block)
    with open('studio_core.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("[+] Successfully patched studio_core.c to invoke studiolens_render_telemetry_dashboard()!")
else:
    print("[!] Could not find exact dashboard block in studio_core.c")

