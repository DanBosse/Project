with open("spatial_compositor.c", "r") as f:
    code = f.read()

# Remove duplicate font table and duplicate draw_char/draw_string if they were appended below, 
# or ensure only one version exists. Let's clean up by rewriting the render_triconjectural_search function.

# Let's verify if draw_string exists around line 120
if "static void draw_string" in code:
    print("Found existing draw_string helper.")

# Let's replace or update render_triconjectural_search to use the existing helpers
new_search_renderer = """
static __attribute__((unused)) void render_triconjectural_search(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height) {
    for (uint32_t dy = 0; dy < height; dy++) {
        for (uint32_t dx = 0; dx < width; dx++) {
            uint32_t px = start_x + dx;
            uint32_t py = start_y + dy;
            if (px < w && py < h) {
                uint32_t bg = ((dx ^ dy) % 32 < 2) ? 0x001E293B : 0x000F172A;
                fb[py * w + px] = bg;
            }
        }
    }
    draw_string(fb, w, h, start_x + 12, start_y + 12, "TRICONJECTURAL SEARCH", 0x0038BDF8);
    draw_string(fb, w, h, start_x + 12, start_y + 36, "Query: manifest", 0x00E2E8F0);
    draw_string(fb, w, h, start_x + 12, start_y + 64, "Index: Tripartite State Active", 0x0094A3B8);
    draw_string(fb, w, h, start_x + 12, start_y + 88, "Match Score: 0.948 (Optimal)", 0x004ADE80);
    draw_string(fb, w, h, start_x + 12, start_y + 112, "Confidence: High [Tier 6]", 0x0094A3B8);
}
"""

# Remove any duplicated font_8x8 or trailing duplicate functions from the bottom
if "static const uint8_t font_8x8" in code:
    # Keep the first font_8x8, remove any second occurrence or clean up
    parts = code.split("static const uint8_t font_8x8")
    code = parts[0] + "static const uint8_t font_8x8" + parts[1] # Keep first
    # If there are more than 2 parts (meaning 2 definitions), keep only up to the second part and append the renderer
    if len(parts) > 2:
        code = parts[0] + "static const uint8_t font_8x8" + parts[1].split("static __attribute__((unused)) void render_triconjectural_search")[0] + new_search_renderer

with open("spatial_compositor.c", "w") as f:
    f.write(code)

print("spatial_compositor.c cleaned and updated!")
