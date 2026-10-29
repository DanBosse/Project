with open("spatial_compositor.c", "r") as f:
    code = f.read()

input_handlers = """
// Mouse and Keyboard State for Spatial Compositor
static int mouse_x = 160;
static int mouse_y = 100;
static uint32_t prev_mouse_buttons = 0;
static int active_window_index = 2; // Default to Triconjectural Search

static void render_cursor(uint32_t *fb, uint32_t w, uint32_t h, int x, int y) {
    for (int dy = 0; dy < 12; dy++) {
        for (int dx = 0; dx < 8; dx++) {
            int px = x + dx;
            int py = y + dy;
            if (px >= 0 && px < (int)w && py >= 0 && py < (int)h) {
                // Simple arrow cursor shape
                if (dx <= dy) {
                    fb[py * w + px] = 0x00FFFFFF; // White cursor
                }
            }
        }
    }
}

void spatial_compositor_mouse_move(int dx, int dy) {
    mouse_x += dx;
    mouse_y += dy;
    if (mouse_x < 0) mouse_x = 0;
    if (mouse_y < 0) mouse_y = 0;
    if (mouse_x >= 320) mouse_x = 319; // Adjust based on standard resolution bounds
    if (mouse_y >= 200) mouse_y = 199;
}

void spatial_compositor_mouse_click(uint32_t button_state) {
    if (button_state && !prev_mouse_buttons) {
        // Hit-test windows to focus
        // If click is within a window header, set active_window_index
        if (mouse_y >= 20 && mouse_y < 180) {
            if (mouse_x < 160 && mouse_y < 100) active_window_index = 0;
            else if (mouse_x >= 160 && mouse_y < 100) active_window_index = 1;
            else if (mouse_x < 160) active_window_index = 2;
            else active_window_index = 3;
        }
    }
    prev_mouse_buttons = button_state;
}

void spatial_compositor_key_press(char c) {
    // Route keypress to active window (e.g. Triconjectural Search query buffer)
    // For now, log or handle active search query input
}
"""

# Append input handlers if not already present
if "spatial_compositor_mouse_move" not in code:
    code = code.strip() + "\n\n" + input_handlers + "\n"
    with open("spatial_compositor.c", "w") as f:
        f.write(code)
    print("Mouse and keyboard input handlers added to spatial_compositor.c!")
else:
    print("Input handlers already present.")
