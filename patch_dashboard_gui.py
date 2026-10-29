with open("studiolens_dashboard.c", "r") as f:
    code = f.read()

# Replace or add spatial_compositor_render call inside studiolens_render_telemetry_dashboard
old_target = """void studiolens_render_telemetry_dashboard(void) {
    spatial_compositor_render(0); // Trigger 5-window spatial compositor view"""

new_target = """void studiolens_render_telemetry_dashboard(void) {
    // Render the 5-window spatial compositor desktop GUI view
    spatial_compositor_render(0);"""

if old_target in code:
    code = code.replace(old_target, new_target)
else:
    # If not found exactly, let's insert it right at the top of the function
    code = code.replace(
        "void studiolens_render_telemetry_dashboard(void) {",
        "void studiolens_render_telemetry_dashboard(void) {\n    spatial_compositor_render(0);"
    )

with open("studiolens_dashboard.c", "w") as f:
    f.write(code)

print("Updated studiolens_dashboard.c successfully!")
