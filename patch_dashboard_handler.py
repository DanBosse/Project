import os

with open('command_registry.c', 'r', encoding='utf-8') as f:
    content = f.read()

# Add extern for studiolens_render_telemetry_dashboard if not present
if "studiolens_render_telemetry_dashboard" not in content:
    content = '#include "studiolens_dashboard.h"\n' + content

# Replace cmd_generic_stub for dashboard with a real handler wrapper
old_entry = '{ "Hardware & Memory",    "dashboard",         "Render StudioLens live telemetry dashboard",        cmd_generic_stub },'
new_entry = '{ "Hardware & Memory",    "dashboard",         "Render StudioLens live telemetry dashboard",        cmd_dashboard_handler },'

# Define cmd_dashboard_handler if not present
handler_def = """
static void cmd_dashboard_handler(const char *arg) {
    (void)arg;
    studiolens_render_telemetry_dashboard();
}
"""

if "cmd_dashboard_handler" not in content:
    # Insert helper before dispatch table or at the top
    content = handler_def + "\n" + content

if old_entry in content:
    content = content.replace(old_entry, new_entry)
    with open('command_registry.c', 'w', encoding='utf-8') as f:
        f.write(content)
    print("[+] Successfully wired 'dashboard' command to studiolens_render_telemetry_dashboard() in command_registry.c!")
else:
    print("[!] Could not find exact dashboard entry string in command_registry.c, checking alternative...")

