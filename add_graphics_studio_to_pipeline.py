with open("physics_studio.c", "r", encoding="utf-8") as f:
    code = f.read()

# Target step 4 / 5 boundary to insert GraphicsStudio explicitly
target = 'serial_print("[Pipeline Step 4/6] AuraVPU Real-Time Ray Tracing & Spatial Rendering...\\n");'

graphics_step = """serial_print("[Pipeline Step 4/7] GraphicsStudio Raymarched Canvas & Topological Manifold Stream...\\n");
    graphics_studio_init();
    serial_print("[GraphicsStudio] High-end 3D raymarched design canvas synchronized to AuraLink Stream.\\n");
    """

# Also update the step numbering from 6 total steps to 7 total steps
if target in code and "Step 7" not in code:
    code = code.replace(target, graphics_step + '    serial_print("[Pipeline Step 5/7] AuraVPU Real-Time Ray Tracing & Spatial Rendering...\\n");')
    
    # Update remaining step labels
    code = code.replace('[Pipeline Step 5/6] AuraLink Backplane Telemetry & Zero-Copy Sync Complete.', '[Pipeline Step 6/7] AuraLink Backplane Telemetry & Zero-Copy Sync Complete.')
    code = code.replace('[Pipeline Step 6/6] StudioLens Spatial Compositor', '[Pipeline Step 7/7] StudioLens Spatial Compositor')
    
    with open("physics_studio.c", "w", encoding="utf-8") as f:
        f.write(code)
    print("GraphicsStudio successfully integrated into pipeline_full as Step 4/7!")
else:
    print("Target already updated or not found.")

