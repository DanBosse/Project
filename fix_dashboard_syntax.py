with open("command_registry.c", "r") as f:
    lines = f.readlines()

new_lines = []
inserted = False
for line in lines:
    if "0x0A" in line or "AuraEngine NPU" in line:
        new_lines.append(line)
        if not inserted:
            # Add the Tier 0x0B line properly formatted for C serial_print
            indent = line[:line.find("[")]
            new_lines.append(f'{indent}serial_print(" [Tier 0x0B] PhysicsLab     : ACTIVE   [Ring 3] | Multi-Body: Active\\n");\n')
            inserted = True
    elif "All 10 Containers" in line:
        new_lines.append(line.replace("All 10 Containers", "All 11 Containers"))
    elif "10 Containers" in line:
        new_lines.append(line.replace("10 Containers", "11 Containers"))
    else:
        new_lines.append(line)

with open("command_registry.c", "w") as f:
    f.writelines(new_lines)

print("Successfully fixed command_registry.c syntax!")
