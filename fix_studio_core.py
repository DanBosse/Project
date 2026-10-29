with open("studio_core.c", "r") as f:
    code = f.read()

# Add unused casts to studio_core_read_pipe parameters
old_func = """size_t studio_core_read_pipe(void *buf, size_t len) {"""
new_func = """size_t studio_core_read_pipe(void *buf, size_t len) {
    (void)buf;
    (void)len;"""

code = code.replace(old_func, new_func)

with open("studio_core.c", "w") as f:
    f.write(code)

print("studio_core.c warnings patched!")
