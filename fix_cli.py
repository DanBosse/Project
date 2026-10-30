with open("cli.c", "r", encoding="utf-8", errors="ignore") as f:
    content = f.read()

# Clean up any broken or old audit blocks
if "trust_sentinel_run_audit" in content:
    # Let us extract and rewrite cli_execute_command cleanly
    pass

# Find the start of cli_execute_command
idx = content.find("void cli_execute_command")
if idx != -1:
    # Let us replace the function body entirely with a clean version
    end_idx = content.find("}", idx)
    # Find the closing brace of cli_execute_command
    brace_count = 0
    body_start = content.find("{", idx)
    curr = body_start
    while curr < len(content):
        if content[curr] == '{':
            brace_count += 1
        elif content[curr] == '}':
            brace_count -= 1
            if brace_count == 0:
                end_idx = curr + 1
                break
        curr += 1
    
    clean_func = """void cli_execute_command(const char *cmd) {
    while (*cmd == ' ') cmd++;
    if (*cmd == '\\0') return;

    if (k_strcmp(cmd, "help") == 0) {
        cli_cmd_help();
    }
    else if (k_strcmp(cmd, "audit") == 0) {
        trust_sentinel_run_audit();
    }
    else if (k_strcmp(cmd, "dashboard") == 0) {
        studiolens_render_telemetry_dashboard();
        spatial_compositor_render();
    }
    else if (k_strcmp(cmd, "status") == 0) {
        cli_cmd_status();
    }
    else if (k_strcmp(cmd, "verify") == 0 || k_strncmp(cmd, "verify ", 7) == 0) {
        cli_cmd_verify();
    }
    else if (k_strcmp(cmd, "tensor") == 0) {
        cli_cmd_tensor();
    }
    else if (k_strcmp(cmd, "pqc") == 0) {
        cli_cmd_pqc();
    }
    else if (k_strncmp(cmd, "search:data ", 12) == 0 ||
             k_strncmp(cmd, "search:net ", 11) == 0 ||
             k_strncmp(cmd, "search:os ", 10) == 0) {
        studiolens_execute_command(cmd);
    }
    else {
        kernel_print("\\n[CLI] Unknown command. Type 'help' for available options.\\n\\n");
    }
}"""
    content = content[:idx] + clean_func + content[end_idx:]
    with open("cli.c", "w", encoding="utf-8") as f:
        f.write(content)
    print("Successfully patched cli_execute_command in cli.c!")
else:
    print("Could not find cli_execute_command in cli.c")
