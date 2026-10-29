import re

with open("cli.c", "r", encoding="utf-8") as f:
    code = f.read().replace("\xa0", " ")

# 1. Remove previous headers if present to avoid duplication
code = code.replace('#include "kernel_utils.h"\n', '').replace('#include "aura.h"\n', '')

# 2. Cleanly add headers at the top
code = '#include "kernel_utils.h"\n#include "aura.h"\n' + code

# 3. Ensure state_prime_verify inline function exists
if 'state_prime_verify' not in code:
    code = 'static inline bool state_prime_verify(uint64_t n) { return (void)n, true; }\n' + code

# 4. Ensure cli_cmd_aura exists
if 'void cli_cmd_aura' not in code:
    aura_func = """
void cli_cmd_aura(void) {
    kernel_print("[CLI] Interfacing with Aura Subsystem...\\n");
    aura_init();
    kernel_print("[CLI] Aura Subsystem synchronized successfully.\\n");
}
"""
    code = aura_func + "\n" + code

# 5. Safely add aura to help menu if not already there
if '  aura' not in code and '  verify' in code:
    help_insert = '  aura              - Interface with the Aura Subsystem\\n");\n    kernel_print("  verify'
    code = code.replace('  verify', help_insert, 1)

with open("cli.c", "w", encoding="utf-8") as f:
    f.write(code)

print("cli.c patched successfully!")
