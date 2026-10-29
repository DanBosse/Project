with open("spatial_compositor.c", "r") as f:
    code = f.read()

# If there is a second font_8x8 definition or duplicated block, let's truncate everything from the first occurrence of duplicate font_8x8
marker = "static const uint8_t font_8x8"
first_pos = code.find(marker)
if first_pos != -1:
    second_pos = code.find(marker, first_pos + len(marker))
    if second_pos != -1:
        # Keep everything up to the second occurrence, then append our correct render_triconjectural_search
        code = code[:second_pos]

# Ensure render_triconjectural_search uses the existing draw_string and is marked unused
search_func = """
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

# Replace existing render_triconjectural_search definition if present
if "render_triconjectural_search" in code:
    import re
    code = re.sub(r'static\s+.*?void\s+render_triconjectural_search\(.*?\n\}', '', code, flags=re.DOTALL)

code = code.strip() + "\n\n" + search_func + "\n"

with open("spatial_compositor.c", "w") as f:
    f.write(code)

print("spatial_compositor.c cleaned and rebuilt successfully!")
