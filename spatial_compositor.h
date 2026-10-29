#ifndef SPATIAL_COMPOSITOR_H
#define SPATIAL_COMPOSITOR_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_WINDOWS 5

typedef struct {
    int id;
    int x, y, width, height;
    char title[32];
    char status_text[32];
    uint32_t color;
    bool active;
} spatial_window_t;

void spatial_compositor_init(uint32_t *fb, uint32_t width, uint32_t height, uint32_t pitch);
void spatial_compositor_render_frame(void);
void spatial_compositor_render(uint32_t mode);
uint32_t* get_linear_framebuffer(void);

void spatial_window_set_title(int id, const char* title);
void spatial_window_set_status(int id, const char* status);
void spatial_window_set_color(int id, uint32_t color);

#endif
