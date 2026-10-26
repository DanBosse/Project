import os

# 1. Fix command_registry.c duplicate definition of cmd_officetelemetry
if os.path.exists("command_registry.c"):
    with open("command_registry.c", "r") as f:
        content = f.read()
    # Remove or comment out the duplicate cmd_officetelemetry definition if present
    old_def = "void cmd_officetelemetry(const char *arg) { cmd_office_telemetry(arg); }"
    if old_def in content:
        content = content.replace(old_def, "/* duplicate removed */")
        with open("command_registry.c", "w") as f:
            f.write(content)
        print("[+] Removed duplicate cmd_officetelemetry from command_registry.c")

# 2. Add office_suite_render to office_suite.c if missing
if os.path.exists("office_suite.c"):
    with open("office_suite.c", "r") as f:
        content = f.read()
    if "office_suite_render" not in content:
        content += "\n\nvoid office_suite_render(void) {\n    // Stub renderer for office suite\n}\n"
        with open("office_suite.c", "w") as f:
            f.write(content)
        print("[+] Added office_suite_render stub to office_suite.c")

# 3. Add terminal_print stub or declaration to sandbox.c if missing
if os.path.exists("sandbox.c"):
    with open("sandbox.c", "r") as f:
        content = f.read()
    if "terminal_print" in content and "void terminal_print" not in content:
        # Prepend a forward declaration or stub implementation if not included
        stub = "extern void terminal_print(const char *str);\n"
        if stub not in content:
            content = stub + content
            with open("sandbox.c", "w") as f:
                f.write(content)
            print("[+] Added terminal_print declaration to sandbox.c")

# 4. Handle multiple definitions between office.c and office_suite.c by turning office.c duplicates static or removing them
if os.path.exists("office.c"):
    with open("office.c", "r") as f:
        content = f.read()
    # If office.c defines cmd_office_launch or cmd_office_comms, make them static or comment out
    content = content.replace("void cmd_office_launch(", "static void cmd_office_launch_local(")
    content = content.replace("void cmd_office_comms(", "static void cmd_office_comms_local(")
    with open("office.c", "w") as f:
        f.write(content)
    print("[+] Resolved duplicate symbols in office.c")

print("Linker fixes applied successfully!")
