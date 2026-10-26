import os

with open('command_registry.c', 'r', encoding='utf-8') as f:
    content = f.read()

# Replace cmd_generic_stub mappings with actual functions where appropriate
replacements = [
    (' { "Hardware & Memory",    "dashboard",         "Render StudioLens live telemetry dashboard",        cmd_generic_stub },',
     ' { "Hardware & Memory",    "dashboard",         "Render StudioLens live telemetry dashboard (Ring 3)", (cmd_handler_t)studiolens_render_telemetry_dashboard },'),
]

for old, new in replacements:
    if old in content:
        content = content.replace(old, new)

with open('command_registry.c', 'w', encoding='utf-8') as f:
    f.write(content)

print("[+] Patched command_registry.c stub bindings!")
