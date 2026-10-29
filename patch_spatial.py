with open("spatial_compositor.c", "r") as f:
    code = f.read()

# 1. Update the interactive window loop/switch statement around line 273
old_switch = """            } else if (i == 2) {
                render_audio_workstation(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 3) {
                render_video_nle(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            }"""

new_switch = """            } else if (i == 2) {
                render_triconjectural_search(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 3) {
                render_audio_workstation(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            } else if (i == 4) {
                render_video_nle(fb, WIDTH, HEIGHT, content_x, content_y, content_w, content_h);
            }"""

if old_switch in code:
    code = code.replace(old_switch, new_switch, 1)
    print("Updated window stack loop switch.")
else:
    print("Warning: old_switch block not found precisely.")

with open("spatial_compositor.c", "w") as f:
    f.write(code)

print("Spatial compositor patched successfully!")
