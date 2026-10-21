with open("kernel.c", "r") as f:
    code = f.read()

# Let's see if render_high_end_shader is already present
if "render_high_end_shader" in code:
    print("[Info] render_high_end_shader already present in kernel.c")
else:
    # Look for where graphicsstudio is handled or where drawing happens
    # We will inject the call right into the graphicsstudio command handler
    print("[Info] Patching kernel.c with render_high_end_shader call...")
    
    # Let's replace the basic rect drawing in graphicsstudio with the shader call
    old_target = 'draw_rect(120, 200, 900, 580, 0x14161C);'
    new_replacement = '// Render high-end raymarched procedural 3D graphics inside the artboard!\n                    render_high_end_shader(120, 200, 900, 580);'
    
    if old_target in code:
        code = code.replace(old_target, new_replacement)
        with open("kernel.c", "w") as f:
            f.write(code)
        print("[Success] kernel.c patched successfully!")
    else:
        print("[Error] Target rectangle draw statement not found in kernel.c")

