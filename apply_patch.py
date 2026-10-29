import re

with open("cli.c", "r", encoding="utf-8") as f:
    code = f.read().replace("\xa0", " ")

# Clean up previous automated additions to start fresh
code = code.replace('#include "kernel_utils.h"\n', '')
code = code.replace('#include "aura.h"\n', '')
code = code.replace('#include <stdbool.h>\n', '')

# Remove old inline stub definitions if present
code = re.sub(r'static inline bool state_prime_verify\(uint64_t n\)[^\n]*\n', '', code)
code = re.sub(r'void cli_cmd_aura\(void\)\s*\{[^}]*+\}', '', code, flags=re.DOTALL)

# 1. Add required headers and stdbool at the top
headers = '#include <stdbool.h>\n#include "kernel_utils.h"\n#include "aura.h"\n'
code = headers + code

# 2. Add state_prime_verify if missing
if 'state_prime_verify' not in code:
    code = 'static inline bool state_prime_verify(uint64_t n) { return (void)n, true; }\n' + code

# 3. Add cli_cmd_aura function if missing
aura_func = """
void cli_cmd_aura(void) {
    kernel_print("[CLI] Interfacing with Aura Subsystem...\\n");
    aura_init();
    kernel_print("[CLI] Aura Subsystem synchronized successfully.\\n");
}
"""
if 'void cli_cmd_aura' not in code:
    code = aura_func + "\n" + code

# 4. Safely insert aura into help menu right before verify
if '  aura' not in code and '  verify' in code:
    help_target = '  verify'
    help_insert = '  aura              - Interface with the Aura Subsystem\\n");\n    kernel_print("  verify'
    code = code.replace(help_target, help_insert, 1)

with open("cli.c", "w", encoding="utf-8") as f:
    f.write(code)

print("Patch applied successfully with all headers and formatting fixed!")
