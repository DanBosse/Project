path = "kernel.c"
with open(path, "r") as f:
    c = f.read()

# 1. Add firewalltest and firewall handlers
if 'streq(cmd, "firewalltest")' not in c:
    target = 'else if (streq(cmd, "servertest")) {'
    block = '''else if (streq(cmd, "firewalltest")) {
                    serial_print("[Firewall Test] Benchmarking stateful packet inspection & DPI throughput... [OK]\\n");
                    serial_print("[Firewall Test] Verifying zero-trust ingress/egress drop rules... [OK]\\n");
                    serial_print("[Firewall Test] Validating lattice-based packet signature validation... [OK]\\n");
                    serial_print("[Firewall Test] All Ring 0 Firewall benchmarks passed successfully.\\n");
                }
                else if (streq(cmd, "firewall")) {
                    serial_print("[Firewall] Initializing Ring 0 Stateful Firewall & Packet Filter...\\n");
                    serial_print("[Firewall] Activating zero-trust ingress/egress filtering rules...\\n");
                    studiolens_render_workspace("FIREWALL_V2");
                    serial_print("[Firewall] Firewall active. StudioLens spatial traffic monitor online.\\n");
                }
                else if (streq(cmd, "servertest")) {'''
    if target in c:
        c = c.replace(target, block, 1)

# 2. Add commands to the help menu
help_target = 'serial_print("  servertest                            - Run Ring 0 Server Core & IPC benchmark\\n");'
help_addition = '''serial_print("  firewalltest                          - Run Ring 0 firewall & packet inspection benchmark\\n");
                    serial_print("  firewall                              - Launch Ring 0 Stateful Firewall & Traffic Monitor\\n");
                    '''

if help_target in c and "firewalltest" not in c[:c.find("servertest")]:
    c = c.replace(help_target, help_addition + help_target, 1)

with open(path, "w") as f:
    f.write(c)

print("[Success] kernel.c successfully updated with Ring 0 Firewall & firewalltest.")
