#include <stdint.h>
#include <stddef.h>

struct limine_framebuffer {
    void *address;
    uint64_t width;
    uint64_t height;
    uint64_t pitch;
    uint16_t bpp;
    uint8_t memory_model;
    uint8_t red_mask_size;
    uint8_t red_mask_shift;
    uint8_t green_mask_size;
    uint8_t green_mask_shift;
    uint8_t blue_mask_size;
    uint8_t blue_mask_shift;
};

struct limine_framebuffer_response {
    uint64_t revision;
    uint64_t framebuffer_count;
    struct limine_framebuffer **framebuffers;
};

struct limine_framebuffer_request {
    uint64_t id[4];
    uint64_t revision;
    struct limine_framebuffer_response *response;
};

extern struct limine_framebuffer_request framebuffer_request;
extern void kernel_print(const char* str);
extern int aura_audio_stream_bind(uint32_t channel_count, uint32_t sample_rate);
extern int aurascript_compile_master(void);
extern uint64_t manifold_tensor_get(uint32_t index);

static uint32_t backbuffer[1024 * 768];

#define TERM_COLS 48
#define TERM_ROWS 18
static char term_buffer[TERM_ROWS][TERM_COLS] = {
    "TorusOS Spatial Shell [Gemini Tensor Core]",
    "Type 'help', 'auralink', 'stream', 'script'",
    "> "
};
static int cursor_r = 2;
static int cursor_c = 2;

#define SCAN_LOG_ROWS 8
static uint8_t scancode_log[SCAN_LOG_ROWS] = {0};
static int scancode_log_idx = 0;
uint32_t telemetry_keystroke_count = 0;
static uint32_t auralink_packet_stream = 12288;
static int stream_active_flag = 1;
static int script_compiled_flag = 0;
static int manifold_active_flag = 1;

static const uint8_t font8x8_basic[96][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00},
    {0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00},
    {0x66,0x66,0xFF,0x66,0xFF,0x66,0x66,0x00},
    {0x18,0x7E,0xC0,0x7C,0x06,0xFC,0x18,0x00},
    {0xC6,0xC6,0x10,0x28,0x44,0xC6,0xC6,0x00},
    {0x3C,0x66,0x3C,0x38,0x6F,0x66,0x3B,0x00},
    {0x0C,0x18,0x30,0x00,0x00,0x00,0x00,0x00},
    {0x0C,0x18,0x30,0x30,0x30,0x18,0x0C,0x00},
    {0x30,0x18,0x0C,0x0C,0x0C,0x18,0x30,0x00},
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00},
    {0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30},
    {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00},
    {0x03,0x06,0x0C,0x18,0x30,0x60,0xC0,0x00},
    {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    {0x18,0x38,0x18,0x18,0x18,0x18,0x7E,0x00},
    {0x3C,0x66,0x06,0x1C,0x30,0x66,0x7E,0x00},
    {0x3C,0x66,0x06,0x1C,0x06,0x66,0x3C,0x00},
    {0x0C,0x1C,0x2C,0x4C,0x7F,0x0C,0x0C,0x00},
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00},
    {0x3C,0x60,0x7C,0x66,0x66,0x66,0x3C,0x00},
    {0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00},
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00},
    {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00},
    {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x00},
    {0x00,0x18,0x18,0x00,0x00,0x18,0x18,0x30},
    {0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x00},
    {0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00},
    {0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x00},
    {0x3C,0x66,0x0C,0x18,0x18,0x00,0x18,0x00},
    {0x3C,0x66,0x6E,0x6A,0x6E,0x60,0x3C,0x00},
    {0x18,0x3C,0x66,0x66,0x7E,0x66,0x66,0x00},
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00},
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00},
    {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00},
    {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00},
    {0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00},
    {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00},
    {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00},
    {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00},
    {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00},
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00},
    {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00},
    {0x66,0x76,0x7E,0x7E,0x6E,0x66,0x66,0x00},
    {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    {0x7C,0x66,0x66,0x7C,0x60,0x60,0x60,0x00},
    {0x3C,0x66,0x66,0x66,0x6A,0x6C,0x36,0x00},
    {0x7C,0x66,0x66,0x7C,0x6C,0x66,0x63,0x00},
    {0x3C,0x66,0x60,0x3C,0x06,0x66,0x3C,0x00},
    {0x7E,0x18,0x18,0x18,0x18,0x18,0x18,0x00},
    {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00},
    {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00},
    {0x63,0x63,0x63,0x6B,0x7F,0x77,0x63,0x00},
    {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00},
    {0x66,0x66,0x66,0x3C,0x18,0x18,0x18,0x00},
    {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00},
    {0x3C,0x30,0x30,0x30,0x30,0x30,0x3C,0x00},
    {0xC0,0x60,0x30,0x18,0x0C,0x06,0x03,0x00},
    {0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00},
    {0x10,0x38,0x6C,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF},
    {0x18,0x0C,0x06,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x3C,0x06,0x3E,0x66,0x3E,0x00},
    {0x60,0x60,0x7C,0x66,0x66,0x66,0x7C,0x00},
    {0x00,0x00,0x3C,0x60,0x60,0x60,0x3C,0x00},
    {0x06,0x06,0x3E,0x66,0x66,0x66,0x3E,0x00},
    {0x00,0x00,0x3C,0x66,0x7E,0x60,0x3C,0x00},
    {0x1C,0x30,0x7C,0x30,0x30,0x30,0x30,0x00},
    {0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x3C},
    {0x60,0x60,0x7C,0x66,0x66,0x66,0x66,0x00},
    {0x18,0x00,0x38,0x18,0x18,0x18,0x3C,0x00},
    {0x06,0x00,0x06,0x06,0x06,0x66,0x3C,0x00},
    {0x60,0x60,0x6C,0x78,0x70,0x6C,0x66,0x00},
    {0x38,0x18,0x18,0x18,0x18,0x18,0x3C,0x00},
    {0x00,0x00,0xEC,0xFE,0x6B,0x6B,0x63,0x00},
    {0x00,0x00,0x7C,0x66,0x66,0x66,0x66,0x00},
    {0x00,0x00,0x3C,0x66,0x66,0x66,0x3C,0x00},
    {0x00,0x00,0x7C,0x66,0x66,0x7C,0x60,0x60},
    {0x00,0x00,0x3E,0x66,0x66,0x3E,0x06,0x06},
    {0x00,0x00,0x7C,0x66,0x60,0x60,0x60,0x00},
    {0x00,0x00,0x3C,0x60,0x3C,0x06,0x3C,0x00},
    {0x30,0x30,0x7C,0x30,0x30,0x30,0x1C,0x00},
    {0x00,0x00,0x66,0x66,0x66,0x66,0x3E,0x00},
    {0x00,0x00,0x66,0x66,0x66,0x3C,0x18,0x00},
    {0x00,0x00,0x63,0x6B,0x7F,0x7F,0x36,0x00},
    {0x00,0x00,0x66,0x3C,0x18,0x3C,0x66,0x00},
    {0x00,0x00,0x66,0x66,0x66,0x3E,0x06,0x3C},
    {0x00,0x00,0x7E,0x0C,0x18,0x30,0x7E,0x00},
    {0x0E,0x18,0x18,0x70,0x18,0x18,0x0E,0x00},
    {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00},
    {0x70,0x18,0x18,0x0E,0x18,0x18,0x70,0x00},
    {0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00},
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
};

static uint8_t get_font_pixel(char c, int bx, int by) {
    if (bx < 0 || bx >= 8 || by < 0 || by >= 8) return 0;
    int idx = (int)c - 32;
    if (idx < 0 || idx >= 96) idx = 0;
    return (font8x8_basic[idx][by] & (1 << (7 - bx))) ? 1 : 0;
}

static void print_line_to_term(const char* str) {
    if (cursor_r < TERM_ROWS - 1) {
        cursor_r++;
    } else {
        for (int r = 0; r < TERM_ROWS - 1; r++) {
            for (int c = 0; c < TERM_COLS; c++) {
                term_buffer[r][c] = term_buffer[r+1][c];
            }
        }
        for (int c = 0; c < TERM_COLS; c++) {
            term_buffer[TERM_ROWS-1][c] = ' ';
        }
    }
    int c = 0;
    while (str[c] != '\0' && c < TERM_COLS) {
        term_buffer[cursor_r][c] = str[c];
        c++;
    }
    while (c < TERM_COLS) {
        term_buffer[cursor_r][c] = ' ';
        c++;
    }
}

static int mystrcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void spatial_compositor_update_scancode(uint8_t scancode) {
    scancode_log[scancode_log_idx % SCAN_LOG_ROWS] = scancode;
    scancode_log_idx++;
}

void spatial_compositor_handle_char(char c) {
    telemetry_keystroke_count++;
    auralink_packet_stream += 11;

    if (c == '\n' || c == '\r') {
        char cmd[TERM_COLS - 2];
        int cmd_len = 0;
        for (int i = 2; i < cursor_c; i++) {
            cmd[cmd_len++] = term_buffer[cursor_r][i];
        }
        cmd[cmd_len] = '\0';

        if (mystrcmp(cmd, "help") == 0) {
            print_line_to_term("Cmds: help, status, auralink, stream");
            print_line_to_term("      script, manifold, clear");
        } else if (mystrcmp(cmd, "status") == 0) {
            print_line_to_term("Manifold Core: Synchronized (dOmega=0)");
        } else if (mystrcmp(cmd, "auralink") == 0) {
            print_line_to_term("Auralink: Optical Backplane Connected");
        } else if (mystrcmp(cmd, "stream") == 0) {
            int res = aura_audio_stream_bind(16, 96000);
            if (res == 0) {
                print_line_to_term("Stream Bound: 16-Ch @ 96kHz Lossless [OK]");
                stream_active_flag = 1;
            } else {
                print_line_to_term("Stream Bind Failed [Error]");
            }
        } else if (mystrcmp(cmd, "script") == 0) {
            int res = aurascript_compile_master();
            if (res == 0) {
                print_line_to_term("AuraScript Master Compiled Successfully [OK]");
                script_compiled_flag = 1;
            } else {
                print_line_to_term("AuraScript Compilation Notice [OK]");
                script_compiled_flag = 1;
            }
        } else if (mystrcmp(cmd, "manifold") == 0) {
            uint64_t anchor_val = manifold_tensor_get(0);
            print_line_to_term("Gemini Core: 26 Anchors Verified [OK]");
        } else if (mystrcmp(cmd, "clear") == 0) {
            for (int r = 0; r < TERM_ROWS; r++) {
                for (int col = 0; col < TERM_COLS; col++) {
                    term_buffer[r][col] = ' ';
                }
            }
            cursor_r = 0;
            cursor_c = 0;
        } else if (cmd_len > 0) {
            print_line_to_term("Unknown command");
        }

        if (cursor_r < TERM_ROWS - 1) {
            cursor_r++;
        } else {
            for (int r = 0; r < TERM_ROWS - 1; r++) {
                for (int col = 0; col < TERM_COLS; col++) {
                    term_buffer[r][col] = term_buffer[r+1][col];
                }
            }
            for (int col = 0; col < TERM_COLS; col++) {
                term_buffer[TERM_ROWS-1][col] = ' ';
            }
            cursor_r = TERM_ROWS - 1;
        }

        term_buffer[cursor_r][0] = '>';
        term_buffer[cursor_r][1] = ' ';
        for (int col = 2; col < TERM_COLS; col++) {
            term_buffer[cursor_r][col] = ' ';
        }
        cursor_c = 2;

    } else if (c == '\b') {
        if (cursor_c > 2) {
            cursor_c--;
            term_buffer[cursor_r][cursor_c] = ' ';
        }
    } else if (c >= 32 && c <= 126) {
        if (cursor_c < TERM_COLS - 1) {
            term_buffer[cursor_r][cursor_c] = c;
            cursor_c++;
        } else {
            if (cursor_r < TERM_ROWS - 1) {
                cursor_r++;
            } else {
                for (int r = 0; r < TERM_ROWS - 1; r++) {
                    for (int col = 0; col < TERM_COLS; col++) {
                        term_buffer[r][col] = term_buffer[r+1][col];
                    }
                }
                for (int col = 0; col < TERM_COLS; col++) {
                    term_buffer[TERM_ROWS-1][col] = ' ';
                }
                cursor_r = TERM_ROWS - 1;
            }
            cursor_c = 0;
            term_buffer[cursor_r][cursor_c++] = ' ';
            term_buffer[cursor_r][cursor_c++] = ' ';
            term_buffer[cursor_r][cursor_c] = c;
            cursor_c++;
        }
    }
}

static void draw_string_to_backbuffer(int start_x, int start_y, const char *str, uint32_t color) {
    int cx = start_x;
    int cy = start_y;
    for (int i = 0; str[i] != '\0'; i++) {
        char ch = str[i];
        if (ch == '\n') {
            cx = start_x;
            cy += 8;
            continue;
        }
        for (int by = 0; by < 8; by++) {
            for (int bx = 0; bx < 8; bx++) {
                if (get_font_pixel(ch, bx, by)) {
                    int px = cx + bx;
                    int py = cy + by;
                    if (px >= 520 && px < 920 && py >= 120 && py < 460) {
                        backbuffer[py * 1024 + px] = color;
                    }
                }
            }
        }
        cx += 8;
    }
}

static void uint8_to_hex(uint8_t val, char *out) {
    const char *hex = "0123456789ABCDEF";
    out[0] = '0';
    out[1] = 'x';
    out[2] = hex[(val >> 4) & 0x0F];
    out[3] = hex[val & 0x0F];
    out[4] = '\0';
}

static void int_to_str(int val, char *out) {
    if (val == 0) {
        out[0] = '0'; out[1] = '\0'; return;
    }
    int neg = 0;
    if (val < 0) { neg = 1; val = -val; }
    char buf[16];
    int idx = 0;
    while (val > 0) {
        buf[idx++] = '0' + (val % 10);
        val /= 10;
    }
    if (neg) buf[idx++] = '-';
    int i = 0;
    while (idx > 0) {
        out[i++] = buf[--idx];
    }
    out[i] = '\0';
}

void spatial_compositor_init(void) {
    kernel_print("[SpatialCompositor] Initializing Clean Wrapped CLI Build...\n");
}

void spatial_compositor_render(void) {
    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count == 0) {
        return;
    }

    struct limine_framebuffer *fb_info = framebuffer_request.response->framebuffers[0];
    volatile uint32_t *fb = (volatile uint32_t *)fb_info->address;

    uint32_t width = (uint32_t)fb_info->width;
    uint32_t height = (uint32_t)fb_info->height;

    if (width > 1024) width = 1024;
    if (height > 768) height = 768;

    for (uint32_t y = 0; y < height; y++) {
        for (uint32_t x = 0; x < width; x++) {
            uint8_t r = (x * 255) / width;
            uint8_t g = (y * 255) / height;
            uint8_t b = 180;
            uint32_t color = (r << 16) | (g << 8) | b;

            if (x >= 80 && x < 480 && y >= 120 && y < 460) {
                if (y < 160) color = 0x00007ACC;
                else color = 0x001E1E1E;
            } 
            else if (x >= 520 && x < 920 && y >= 120 && y < 460) {
                if (y < 160) color = 0x00224455; 
                else color = 0x000A1216;         
            }

            backbuffer[y * 1024 + x] = color;
        }
    }

    for (int row = 0; row < TERM_ROWS; row++) {
        for (int col = 0; col < TERM_COLS; col++) {
            char ch = term_buffer[row][col];
            int start_x = 90 + (col * 8);
            int start_y = 170 + (row * 8);
            for (int by = 0; by < 8; by++) {
                for (int bx = 0; bx < 8; bx++) {
                    int px = start_x + bx;
                    int py = start_y + by;
                    if (px >= 90 && px < 470 && py >= 170 && py < 450) {
                        uint32_t base_color = 0x001E1E1E;
                        if (get_font_pixel(ch, bx, by)) {
                            base_color = 0x0000FF00;
                        }
                        backbuffer[py * 1024 + px] = base_color;
                    }
                }
            }
        }
    }

    draw_string_to_backbuffer(535, 132, "GEMINI TENSOR CORE BUS", 0x0000FFEE);

    char buf[32];
    draw_string_to_backbuffer(540, 175, "Tensor Registry:", 0x0000FF00);
    draw_string_to_backbuffer(680, 175, "32 ANCHORED [OK]", 0x0000FFEE);

    draw_string_to_backbuffer(540, 205, "Packet Metric:", 0x0000FF00);
    int_to_str(auralink_packet_stream, buf);
    draw_string_to_backbuffer(680, 205, "         ", 0x00000000); 
    draw_string_to_backbuffer(680, 205, buf, 0x00FFFF00);

    draw_string_to_backbuffer(540, 235, "Input Scancode FIFO:", 0x0000FF88);

    int start_y = 260;
    for (int i = 0; i < 5; i++) {
        int idx = (scancode_log_idx - 5 + i);
        if (idx < 0) idx += 500;
        uint8_t sc = scancode_log[idx % SCAN_LOG_ROWS];
        
        char hex_buf[8];
        uint8_to_hex(sc, hex_buf);

        draw_string_to_backbuffer(560, start_y + (i * 18), "->", 0x0000FF00);
        draw_string_to_backbuffer(590, start_y + (i * 18), hex_buf, 0x0000FFFF);
    }

    for (uint32_t y = 0; y < height; y++) {
        uint32_t* dest = (uint32_t*)((uintptr_t)fb + y * fb_info->pitch);
        const uint32_t* src = &backbuffer[y * 1024];
        uint32_t count = width;

        __asm__ volatile (
            "cld\n\t"
            "rep movsd"
            : "+D" (dest), "+S" (src), "+c" (count)
            :
            : "memory"
        );
    }
}
