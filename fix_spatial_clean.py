with open("spatial_compositor.c", "r") as f:
    code = f.read()

# Remove any previous broken insertions
if "render_triconjectural_search" in code:
    import re
    code = re.sub(r'static void render_triconjectural_search\(.*?\n\}', '', code, flags=re.DOTALL)

# Clean up multiple newlines
code = re.sub(r'\n{3,}', '\n\n', code)

# Define the function properly
search_func = """static void render_triconjectural_search(uint32_t *fb, uint32_t w, uint32_t h, uint32_t start_x, uint32_t start_y, uint32_t width, uint32_t height) {
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

# Insert right after the header includes / helper declarations (before spatial_compositor_render)
target = "static void spatial_compositor_render("
if target in code:
    code = code.replace(target, search_func + "\n\n" + target, 1)
    with open("spatial_compositor.c", "w") as f:
        f.write(code)
    print("Successfully injected render_triconjectural_search!")
else:
    print("Error: Target render function not found.")
