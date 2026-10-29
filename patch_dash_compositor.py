with open("studiolens_dashboard.c", "r") as f:
    code = f.read()

if '#include "spatial_compositor.h"' not in code:
    code = '#include "spatial_compositor.h"\n' + code

target = "void studiolens_render_telemetry_dashboard(void) {"
replacement = target + "\n    spatial_compositor_render(0); // Trigger 5-window spatial compositor view"

if "spatial_compositor_render(0)" not in code:
    code = code.replace(target, replacement)
    with open("studiolens_dashboard.c", "w") as f:
        f.write(code)
    print("Patched studiolens_dashboard.c successfully!")
else:
    print("Already patched.")
