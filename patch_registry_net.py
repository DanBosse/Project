with open('command_registry.c', 'r', encoding='utf-8') as f:
    content = f.read()

net_commands_impl = """
void cmd_ping(const char *arg) {
    (void)arg;
    net_send_ping(0x0A000202); // 10.0.2.2 gateway
}

void cmd_netstat(const char *arg) {
    (void)arg;
    net_print_status();
}
"""

if "void cmd_ping" not in content:
    content = content.replace("static const command_entry_t command_table[] =", net_commands_impl + "\nstatic const command_entry_t command_table[] =")

target_entry = '{ "Hardware & Memory",    "sandbox",           "Enforce Ring 3/0 secure isolation boundaries",      cmd_sandbox },'
new_entries = """    { "Network & Connectivity", "ping",              "Send ICMP echo request to gateway",                 cmd_ping },
    { "Network & Connectivity", "netstat",           "Display TorusOS network stack statistics",          cmd_netstat },
    { "Hardware & Memory",    "sandbox",           "Enforce Ring 3/0 secure isolation boundaries",      cmd_sandbox },"""

if '"ping"' not in content:
    content = content.replace(target_entry, new_entries)

# Ensure no banned terms exist anywhere in the registry file content
content = content.replace("Sovereign", "Independent").replace("sovereign", "independent")

with open('command_registry.c', 'w', encoding='utf-8') as f:
    f.write(content)

print("[+] Patched command_registry.c successfully!")
