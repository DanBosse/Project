with open("kernel.c", "r") as f:
    code = f.read()

target = 'else if (streq(cmd, "graphicsstudio")) {'
replacement = '''else if (streq(cmd, "graphicsstudio")) {
                    serial_print("[StudioLens Graphics] Initializing GPU-accelerated graphics canvas...\\n");
                    serial_print("[StudioLens Graphics] Enabling Wide Color Gamut, Log Color Profiles & Neural Upscale...\\n");
                    studiolens_render_workspace("GRAPHICSSTUDIO_CANVAS_V2");
                    serial_print("[StudioLens Graphics] Canvas upgrade & VTX raster pipeline active.\\n");'''

if target in code:
    # Replace up to the closing brace of that block
    idx = code.find(target)
    end_idx = code.find("}", idx) + 1
    code = code[:idx] + replacement + code[end_idx:]
    with open("kernel.c", "w") as f:
        f.write(code)
    print("[Success] Graphics upgrade applied cleanly.")
else:
    print("[Error] Target block not found.")
