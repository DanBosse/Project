#ifndef STUDIOLENS_H
#define STUDIOLENS_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LAYOUT_SINGLE,
    LAYOUT_GRID,
    LAYOUT_STACKED,
    LAYOUT_FLOATING
} WindowLayoutMode;

void studiolens_init(void);
void studiolens_render_workspace(const char* workspace_name);
void studiolens_set_layout(WindowLayoutMode mode);
void studiolens_spawn_window(const char* title, uint32_t x, uint32_t y, uint32_t w, uint32_t h);

#endif
