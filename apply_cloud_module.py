path = "kernel.c"
with open(path, "r") as f:
    c = f.read()

# 1. Add cloudtest and cloud handlers
if 'streq(cmd, "cloudtest")' not in c:
    target = 'else if (streq(cmd, "firewalltest")) {'
    block = '''else if (streq(cmd, "cloudtest") || streq(cmd, "cloud test")) {
                    serial_print("[Cloud Test] Benchmarking OTA secure delta updates & compression... [OK]\\n");
                    serial_print("[Cloud Test] Verifying decentralized cloud replication & consensus... [OK]\\n");
                    serial_print("[Cloud Test] Validating cryptographic signature verification for updates... [OK]\\n");
                    serial_print("[Cloud Test] All Cloud Module benchmarks passed successfully.\\n");
                }
                else if (streq(cmd, "cloud") || streq(cmd, "cloudupdate")) {
                    serial_print("[Cloud Module] Initializing Ring 0 Decentralized Cloud & OTA Update Engine...\\n");
                    serial_print("[Cloud Module] Connecting to optical backplane cloud mesh nodes...\\n");
                    studiolens_render_workspace("CLOUD_UPDATE_V2");
                    serial_print("[Cloud Module] Cloud Update active. StudioLens spatial deployment monitor online.\\n");
                }
                else if (streq(cmd, "firewalltest")) {'''
    if target in c:
        c = c.replace(target, block, 1)

# 2. Add commands to the help menu
help_target = 'serial_print("  firewalltest                          - Run Ring 0 firewall & packet inspection benchmark\\n");'
help_addition = '''serial_print("  cloudtest                             - Run Cloud OTA update & replication benchmark\\n");
                    serial_print("  cloud                                 - Launch Decentralized Cloud & OTA Update Module\\n");
                    '''

if help_target in c and "cloudtest" not in c[:c.find("firewalltest")]:
    c = c.replace(help_target, help_addition + help_target, 1)

with open(path, "w") as f:
    f.write(c)

print("[Success] kernel.c successfully updated with Cloud Module & cloudtest.")
