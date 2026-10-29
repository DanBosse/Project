import os

kernel_file = None
for f in os.listdir("."):
    if f.endswith(".c") and any(k in f.lower() for k in ["kernel", "main", "boot", "init"]):
        kernel_file = f
        break

if kernel_file:
    print(f"Found kernel entry file: {kernel_file}")
    with open(kernel_file, "r") as f:
        content = f.read()
    
    # Check if compositor init is already there
    if "spatial_compositor_init" not in content:
        # Add extern declaration and initialization call
        externs = "extern void spatial_compositor_init(uint32_t *fb, uint32_t width, uint32_t height, uint32_t pitch);\nextern void spatial_compositor_render_frame(void);\n"
        
        # Find a good spot (e.g., after includes)
        import re
        last_include = max([m.end() for m in re.finditer(r'#include\s+<[^>]+>', content)], default=0)
        content = content[:last_include] + "\n\n" + externs + content[last_include:]
        
        with open(kernel_file, "w") as f:
            f.write(content)
        print(f"Added compositor declarations to {kernel_file}!")
    else:
        print("Compositor already referenced in kernel file.")
else:
    print("Could not automatically locate kernel entry file. Please specify your main kernel C file name.")
