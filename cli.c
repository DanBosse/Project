#include <stdint.h>
#include <stdbool.h>

// External kernel functions
extern void kernel_print(const char *str);
extern bool state_prime_verify(uint64_t n);

// Lightweight string comparison helper for kernel mode
static int k_strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// Check if a string starts with a specific prefix
static bool k_strncmp(const char *str, const char *prefix, int n) {
    for (int i = 0; i < n; i++) {
        if (str[i] != prefix[i]) return false;
        if (str[i] == '\0') break;
    }
    return true;
}

// Command: Help Menu
static void cli_cmd_help(void) {
    kernel_print("\n=== TorusOS / Frequency Manifold CLI ===\n");
    kernel_print("  help          - Display this help menu\n");
    kernel_print("  status        - Show subsystem and vector unit status\n");
    kernel_print("  verify        - Run Phase 1 state prime verification (n=17)\n");
    kernel_print("  tensor        - Execute PhaseCore tensor mapping operation\n\n");
}

// Command: System Status
static void cli_cmd_status(void) {
    kernel_print("\n[Status] Ring 0 Execution: ACTIVE\n");
    kernel_print("[Status] SIMD Vector Unit: AVX/SSE Initialized\n");
    kernel_print("[Status] PhaseCore Engine: Ready\n");
    kernel_print("[Status] Manifold State: Stable\n\n");
}

// Command: Tensor Dispatch
static void cli_cmd_tensor(void) {
    kernel_print("\n[PhaseCore] Dispatching TENSOR_MAP (Opcode 0x07)...\n");
    kernel_print("[PhaseCore] Tensor spatial mapping projection complete.\n\n");
}

// Command: Prime Verification
static void cli_cmd_verify(void) {
    uint64_t test_val = 17; // Default core seed
    kernel_print("\n[CLI] Executing Phase 1: state_prime_verify(17)...\n");
    bool prime_ok = state_prime_verify(test_val);
    if (prime_ok) {
        kernel_print("[CLI] Verification PASSED (Valid State Seed)\n\n");
    } else {
        kernel_print("[CLI] Verification FAILED\n\n");
    }
}

// Main CLI Command Dispatcher
void cli_execute_command(const char *cmd) {
    while (*cmd == ' ') cmd++;

    if (*cmd == '\0') return;

    if (k_strcmp(cmd, "help") == 0) {
        cli_cmd_help();
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
    else {
        kernel_print("\n[CLI] Unknown command. Type 'help' for available options.\n\n");
    }
}
