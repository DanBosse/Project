with open("cli.c", "r") as f:
    content = f.read()

# Add extern declaration for spatial compositor render if not present
ext_decl = "extern void spatial_compositor_render(void);\n"
if "spatial_compositor_render" not in content:
    # Insert near other externs
    content = content.replace("extern void studiolens_execute_command(const char *args);", 
                              "extern void studiolens_execute_command(const char *args);\n" + ext_decl)

# Update the dashboard command handler
old_handler = """    else if (k_strcmp(cmd, "dashboard") == 0) {
        studiolens_render_telemetry_dashboard();
    }"""

new_handler = """    else if (k_strcmp(cmd, "dashboard") == 0) {
        studiolens_render_telemetry_dashboard();
        spatial_compositor_render();
    }"""

if old_handler in content:
    content = content.replace(old_handler, new_handler)
    with open("cli.c", "w") as f:
        f.write(content)
    print("Successfully updated cli.c to trigger spatial_compositor_render()!")
else:
    print("Could not find exact dashboard block in cli.c. Please check structure.")
