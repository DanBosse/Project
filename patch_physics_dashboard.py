with open("command_registry.c", "r") as f:
    content = f.read()

tier_a_line = None
for line in content.splitlines():
    if "0x0A" in line or "AuraEngine NPU" in line:
        tier_a_line = line
        break

if tier_a_line:
    indent = tier_a_line[:tier_a_line.find("[")]
    tier_b_line = f'{indent}[Tier 0x0B] PhysicsLab     : ACTIVE   [Ring 3] | Multi-Body: Active'
    
    new_content = content.replace(tier_a_line, f"{tier_a_line}\n{tier_b_line}")
    new_content = new_content.replace("All 10 Containers", "All 11 Containers")
    new_content = new_content.replace("10 Containers", "11 Containers")
    
    with open("command_registry.c", "w") as f:
        f.write(new_content)
    print("Successfully patched command_registry.c with Tier 0x0B!")
else:
    print("Could not find Tier 0x0A entry in command_registry.c")
