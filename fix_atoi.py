with open("command_registry.c", "r") as f:
    code = f.read()

# Remove any previous attempts at simple_atoi
while "static int simple_atoi" in code:
    parts = code.split("static int simple_atoi", 1)
    header = parts[0]
    remainder = parts[1]
    # Find the closing brace of simple_atoi
    brace_count = 0
    end_idx = 0
    for i, char in enumerate(remainder):
        if char == '{':
            brace_count += 1
        elif char == '}':
            if brace_count == 0:
                end_idx = i
                break
            brace_count -= 1
    if end_idx > 0:
        rest = remainder[end_idx+1:]
    else:
        rest = remainder.split("\n\n", 1)[1] if "\n\n" in remainder else remainder
    code = header + rest

# Clean implementation of simple_atoi
clean_func = """
static int simple_atoi(const char *str) {
    int res = 0;
    if (!str) return 0;
    while (*str == ' ' || *str == '\\t') str++;
    while (*str >= '0' && *str <= '9') {
        res = res * 10 + (*str - '0');
        str++;
    }
    return res;
}
"""

# Insert right after standard includes
target = "#include <stddef.h>"
if target in code:
    code = code.replace(target, target + "\n" + clean_func)
else:
    code = clean_func + code

with open("command_registry.c", "w") as f:
    f.write(code)

print("command_registry.c successfully updated with clean simple_atoi.")
