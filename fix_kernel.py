with open("kernel.c", "r") as f:
    content = f.read()

# Remove old inline AuraMotionEngine and functions if present
import re
# We can declare external reference instead
if "void auramotion_init_adaptive" not in content:
    pass

print("Patching kernel.c...")
