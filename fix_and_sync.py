# 1. Update spatial_compositor.c to eliminate sign comparison warnings
with open("spatial_compositor.c", "r") as f:
    code = f.read()

# Fix the render_counter comparison
code = code.replace(
    '((render_counter / 15) % MAX_WINDOWS == (uint32_t)i)',
    '((int)((render_counter / 15) % MAX_WINDOWS) == i)'
)

with open("spatial_compositor.c", "w") as f:
    f.write(code)

print("spatial_compositor.c type warnings resolved!")
