with open("command_registry.c", "r") as f:
    code = f.read()

# Replace the category matching loop with a properly scoped index
old_snippet = """            int j = 0;
            for (int j = 0; cat_buf[j] != '\\0' && c[j] != '\\0'; j++) {
                if (cat_buf[j] != c[j]) { match = false; break; }
            }
            if (cat_buf[j] != '\\0' || c[j] != '\\0') match = false;"""

# Let us do a robust replacement
import re
new_snippet = """            int j = 0;
            while (cat_buf[j] != '\\0' && c[j] != '\\0') {
                if (cat_buf[j] != c[j]) { match = false; break; }
                j++;
            }
            if (cat_buf[j] != '\\0' || c[j] != '\\0') match = false;"""

code = re.sub(r'for\s*\(int\s+j\s*=\s*0;\s*cat_buf\[j\][^\)]+\)\s*\{[^}]+\}\s*if\s*\(cat_buf\[j\][^\)]+\)\s*match\s*=\s*false;', new_snippet, code)

with open("command_registry.c", "w") as f:
    f.write(code)

print("Patched scope in command_registry.c")
