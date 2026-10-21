path = "kernel.c"
with open(path, "r") as f:
    c = f.read()

# Add support for "firewall test" with a space
old_target = 'else if (streq(cmd, "firewalltest")) {'
new_target = '''else if (streq(cmd, "firewalltest") || streq(cmd, "firewall test")) {'''

if old_target in c and "firewall test" not in c:
    c = c.replace(old_target, new_target, 1)

with open(path, "w") as f:
    f.write(c)

print("[Success] kernel.c updated to support 'firewall test' with a space.")
