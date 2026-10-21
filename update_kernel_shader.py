with open("kernel.c", "r") as f:
    code = f.read()

# Replace the graphicsstudio block with one that calls render_high_end_shader
old_block = '''else if (streq(cmd, "graphicsstudio")) {'''

# Let's find and replace or inject the shader call
target = 'else if (streq(cmd, "graphicsstudio")) {'
replacement = '''else if (streq(cmd, "graphicsstudio")) {
                    serial_print("[Graphics Studio] Initializing Wide-Gamut Neural Design Canvas...\\n");
                    clear_screen(0x0A0B10); // Deep obsidian background
                    draw_rect(100, 100, 1200, 700, 0x1E222A); // Main canvas viewport
                    draw_rect(120, 120, 1160, 60, 0x2A2E39);  // Toolbar header
                    
                    // Render high-end raymarched procedural 3D graphics inside the artboard!
                    render_high_end_shader(120, 200, 900, 580); 
                    
                    draw_rect(1040, 200, 240, 580, 0x171A21); // Inspector panel
                    serial_print("[Graphics Studio] High-end raymarched shader active on bare metal.\\n");
                '''

if 'render_high_end_shader' not in code:
    # If old block exists, replace it
    if target in code:
        # Find the end of that block or replace the whole handler
        # Let's do a safe search-and-replace for the graphicsstudio handler
        print("[Info] Injecting raymarched shader call into kernel.c...")
        # Let's write a targeted patch
    else:
        print("[Warning] graphicsstudio command block not found cleanly in kernel.c")

