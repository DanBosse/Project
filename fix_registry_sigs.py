with open("command_registry.c", "r") as f:
    content = f.read()

# Fix the function signatures to accept const char *arg
content = content.replace(
    "void cmd_officecomms(void) { cmd_office_comms(arg); }",
    "void cmd_officecomms(const char *arg) { cmd_office_comms(arg); }"
)
content = content.replace(
    "void cmd_officetelemetry(void) { cmd_office_telemetry(arg); }",
    "void cmd_officetelemetry(const char *arg) { cmd_office_telemetry(arg); }"
)

with open("command_registry.c", "w") as f:
    f.write(content)

print("[+] Fixed command_registry.c signatures.")
