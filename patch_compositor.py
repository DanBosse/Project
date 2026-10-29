with open("spatial_compositor.c", "r") as f:
    code = f.read()

text_renderer_code = """
static const uint8_t font_8x8[128][8] = {
    [' '] = {0,0,0,0,0,0,0,0},
    ['.'] = {0,0,0,0,0,24,24,0},
    [':'] = {0,0,24,24,0,24,24,0},
    ['0'] = {12,30,51,51,51,51,30,12},
    ['1'] = {12,14,12,12,12,12,12,63},
    ['2'] = {30,51,3,6,12,24,48,63},
    ['3'] = {30,51,3,14,3,51,30,0},
    ['4'] = {6,14,26,50,63,2,2,0},
    ['5'] = {63,48,48,62,3,3,30,0},
    ['6'] = {14,24,48,62,51,51,30,0},
    ['7'] = {63,3,6,12,24,24,24,0},
    ['8'] = {30,51,51,30,51,51,30,0},
    ['9'] = {30,51,51,31,3,6,28,0},
    ['A'] = {12,30,51,51,63,51,51,0},
    ['B'] = {62,51,51,62,51,51,62,0},
    ['C'] = {30,51,48,48,48,51,30,0},
    ['D'] = {60,54,51,51,51,54,60,0},
    ['E'] = {63,48,48,62,48,48,63,0},
    ['F'] = {63,48,48,62,48,48,48,0},
    ['G'] = {30,51,48,59,51,51,30,0},
    ['H'] = {51,51,51,63,51,51,51,0},
    ['I'] = {30,12,12,12,12,12,30,0},
    ['J'] = {3,3,3,3,3,51,30,0},
    ['K'] = {51,53,57,62,57,53,51,0},
    ['L'] = {48,48,48,48,48,48,63,0},
    ['M'] = {51,65,65,65,65,65,65,0},
    ['N'] = {51,59,63,55,51,51,51,0},
    ['O'] = {30,51,51,51,51,51,30,0},
    ['P'] = {62,51,51,62,48,48,48,0},
    ['Q'] = {30,51,51,51,57,30,7,0},
    ['R'] = {62,51,51,62,54,51,51,0},
    ['S'] = {30,48,48,30,3,3,30,0},
    ['T'] = {63,12,12,12,12,12,12,0},
    ['U'] = {51,51,51,51,51,51,30,0},
    ['V'] = {51,51,51,51,30,30,12,0},
    ['W'] = {65,65,65,65,73,85,65,0},
    ['X'] = {51,51,30,12,30,51,51,0},
    ['Y'] = {51,51,51,30,12,12,12,0},
    ['Z'] = {63,3,6,12,24,48,63,0},
    ['a'] = {0,0,30,3,31,51,30,0},
    ['b'] = {48,48,58,51,51,51,58,0},
    ['c'] = {0,0,30,48,48,48,30,0},
    ['d'] = {3,3,31,51,51,51,31,0},
    ['e'] = {0,0,30,51,63,48,30,0},
    ['f'] = {14,24,62,24,24,24,24,0},
    ['g'] = {0,30,51,51,31,3,30,48},
    ['h'] = {48,48,58,51,51,51,51,0},
    ['i'] = {24,0,56,24,24,24,60,0},
    ['j'] = {6,0,14,6,6,6,54,36},
    ['k'] = {48,48,53,57,62,57,51,0},
    ['l'] = {56,24,24,24,24,24,60,0},
    ['m'] = {0,0,58,85,85,85,85,0},
    ['n'] = {0,0,58,51,51,51,51,0},
    ['o'] = {0,0,30,51,51,51,30,0},
    ['p'] = {0,58,51,51,58,48,48,0},
    ['q'] = {0,31,51,51,31,3,3,0},
    ['r'] = {0,0,58,51,48,48,48,0},
    ['s'] = {0,0,30,48,30,3,30,0},
    ['t'] = {24,24,62,24,24,24,14,0},
    ['u'] = {0,0,51,51,51,51,31,0},
    ['v'] = {0,0,51,51,51,30,12,0},
    ['w'] = {0,0,65,65,85,85,34,0},
    ['x'] = {0,0,51,30,12,30,51,0},
    ['y'] = {0,51,51,51,31,3,30,48},
    ['z'] = {0,0,63,6,12,24,63,0}
};

static void draw_char(uint32_t *fb, uint32_t w, uint32_t h, uint32_t x, uint32_t y, char c, uint32_t color) {
    unsigned char uc = (unsigned char)c;
    if (uc >= 128) return;
    for (uint32_t row = 0; row < 8; row++) {
        uint8_t bits = font_8x8[uc][row];
        for (uint32_t col = 0; col < 8; col++) {
            if (bits & (1 << (7 - col))) {
                uint32_t px = x + col;
                uint32_t py = y + row;
                if (px < w && py < h) {
                    fb[py * w + px] = color;
                }
            }
        }
    }
}

static void draw_string(uint32_t *fb, uint32_t w, uint32_t h, uint32_t x, uint32_t y, const char *str, uint32_t color) {
    uint32_t cur_x = x;
    while (*str) {
        if (*str == '\\n') {
            y += 10;
            cur_x = x;
        } else {
            draw_char(fb, w, h, cur_x, y, *str, color);
            cur_x += 8;
        }
        str++;
    }
}

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

if "static __attribute__((unused)) void render_triconjectural_search" in code:
    start_idx = code.find("static __attribute__((unused)) void render_triconjectural_search")
else:
    start_idx = code.find("static void render_triconjectural_search")

end_idx = code.find("static void render_telemetry_dashboard", start_idx)

if start_idx != -1 and end_idx != -1:
    code = code[:start_idx] + text_renderer_code + "\n" + code[end_idx:]
    with open("spatial_compositor.c", "w") as f:
        f.write(code)
    print("Successfully patched spatial_compositor.c with font renderer!")
else:
    print("Could not locate function boundaries.")
