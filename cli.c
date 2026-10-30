#include <stdint.h>
#include <stddef.h>
#include "kernel_utils.h"

// External command handler declarations
extern void cmd_firewall(const char *arg);
extern void cmd_trustreg(void);
extern void cmd_containers(void);
extern void cmd_container_mgmt(const char *arg);
extern void cmd_syslog_clear(void);
extern void cmd_ls(void);
extern void cmd_cat(const char *arg);
extern void cmd_touch(const char *arg);
extern void cmd_write(const char *arg);
extern void cmd_rm(const char *arg);
extern void cmd_vfs_reset(void);
extern void cmd_vfs_export(void);
extern void cmd_np_flood(void);
extern void cmd_heaptest(void);
extern void cmd_ps(void);
extern void cmd_meminfo(void);
extern void cmd_stats(void);
extern void cmd_netstat(void);
extern void cmd_dmesg(void);
extern void cmd_uptime(void);
extern void cmd_version(void);
extern void cmd_clear(void);
extern void cmd_help(void);

static int local_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void cli_execute(char *input) {
    if (!input || !*input) return;

    // Skip leading spaces
    while (*input == ' ') input++;
    if (!*input) return;

    // Split command and argument at the first space
    char *cmd = input;
    char *arg = NULL;
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] == ' ') {
            input[i] = '\0';
            arg = &input[i + 1];
            while (*arg == ' ') arg++; // skip extra spaces
            break;
        }
    }

    // Dispatch commands
    if (!local_strcmp(cmd, "firewall")) {
        cmd_firewall(arg);
    } else if (!local_strcmp(cmd, "trustreg")) {
        cmd_trustreg();
    } else if (!local_strcmp(cmd, "containers")) {
        cmd_containers();
    } else if (!local_strcmp(cmd, "container")) {
        cmd_container_mgmt(arg);
    } else if (!local_strcmp(cmd, "syslog")) {
        if (arg && !local_strcmp(arg, "clear")) cmd_syslog_clear();
        else kernel_print("Usage: syslog clear\n");
    } else if (!local_strcmp(cmd, "ls")) {
        cmd_ls();
    } else if (!local_strcmp(cmd, "cat")) {
        cmd_cat(arg);
    } else if (!local_strcmp(cmd, "touch")) {
        cmd_touch(arg);
    } else if (!local_strcmp(cmd, "write")) {
        cmd_write(arg);
    } else if (!local_strcmp(cmd, "rm")) {
        cmd_rm(arg);
    } else if (!local_strcmp(cmd, "vfs_reset")) {
        cmd_vfs_reset();
    } else if (!local_strcmp(cmd, "vfs_export")) {
        cmd_vfs_export();
    } else if (!local_strcmp(cmd, "np_flood")) {
        cmd_np_flood();
    } else if (!local_strcmp(cmd, "heaptest")) {
        cmd_heaptest();
    } else if (!local_strcmp(cmd, "ps")) {
        cmd_ps();
    } else if (!local_strcmp(cmd, "meminfo")) {
        cmd_meminfo();
    } else if (!local_strcmp(cmd, "stats")) {
        cmd_stats();
    } else if (!local_strcmp(cmd, "netstat")) {
        cmd_netstat();
    } else if (!local_strcmp(cmd, "dmesg")) {
        cmd_dmesg();
    } else if (!local_strcmp(cmd, "uptime")) {
        cmd_uptime();
    } else if (!local_strcmp(cmd, "version")) {
        cmd_version();
    } else if (!local_strcmp(cmd, "clear")) {
        cmd_clear();
    } else if (!local_strcmp(cmd, "help") || !local_strcmp(cmd, "dashboard")) {
        cmd_help();
    } else {
        kernel_print("Unknown command: '");
        kernel_print(cmd);
        kernel_print("'. Type 'help' for available commands.\n");
    }
}
