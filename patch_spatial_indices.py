with open("spatial_compositor.c", "r") as f:
    code = f.read()

old_block = """            if (i == 0) {
                render_embedded_shader(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 1) {
                render_telemetry_dashboard(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 2) {
                render_triconjectural_search(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 3) {
                render_audio_workstation(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 4) {
                render_video_nle(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            }"""

new_block = """            if (i == 0) {
                render_embedded_shader(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h); // Graphics Studio
            } else if (i == 1) {
                render_telemetry_dashboard(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h); // Telemetry Panel
            } else if (i == 2) {
                render_audio_workstation(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h); // Audio Workstation
            } else if (i == 3) {
                render_video_nle(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h); // Video NLE Timeline
            }"""

if old_block in code:
    code = code.replace(old_block, new_block)
    with open("spatial_compositor.c", "w") as f:
        f.write(code)
    print("Successfully patched spatial_compositor.c indices!")
else:
    print("Could not find exact block to replace. Checking alternative pattern...")
    # Fallback search if spacing differs slightly
    if "render_triconjectural_search" in code:
        # Let's do a broader replacement or manual check
        print("Please inspect spatial_compositor.c around render_triconjectural_search.")
