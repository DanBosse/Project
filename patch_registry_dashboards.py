#!/usr/bin/env python3
import os

print("[TORUS] Inspecting and patching master registry with dashboards & subsystems...")

# Let's verify command_registry.c exists
if not os.path.exists('command_registry.c'):
    print("[ERROR] command_registry.c not found!")
    exit(1)

with open('command_registry.c', 'r', encoding='utf-8') as f:
    content = f.read()

# Check if studiolens dashboard or spatial compositor are already referenced
if "studiolens_dashboard" not in content and "studiolens_init" in content:
    print("[TORUS] Injecting StudioLens, Spatial Compositor, and Dashboard hooks...")
    
    # Add extern declarations right before command entries
    target_str = "// External Subsystem Function Hooks"
    hooks_to_add = """
// Extended Subsystem & Dashboard Hooks
extern void studiolens_init(void);
extern void studiolens_execute_command(const char *args);
extern void spatial_compositor_init(void);
extern void spatial_compositor_render(uint32_t flags);
extern void physics_studio_init(void);
extern void physics_studio_full_pipeline(void);
extern void synthesizer_init(void);
extern uint64_t synthesizer_generate_axiom(uint64_t input_genus);
"""
    if target_str in content:
        content = content.replace(target_str, target_str + hooks_to_add)
        with open('command_registry.c', 'w', encoding='utf-8') as f:
            f.write(content)
        print("[+] Successfully patched command_registry.c with missing dashboard and subsystem externs!")
    else:
        print("[!] Target string for insertion not found in command_registry.c")
else:
    print("[+] Subsystem hooks appear to already be integrated in command_registry.c")

