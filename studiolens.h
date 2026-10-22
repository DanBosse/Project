#ifndef STUDIOLENS_H
#define STUDIOLENS_H

#include <stdint.h>
#include <stddef.h>

typedef enum {
    LAYOUT_GRID,
    LAYOUT_STACKED,
    LAYOUT_FLOATING
} WindowLayoutMode;

// Initialize the Tripartite StudioLens Environment
void studiolens_init(void);

// Layout and Workspace management
void studiolens_set_layout(WindowLayoutMode mode);
void studiolens_set_workspace(const char* ws_name);
void studiolens_spawn_window(const char* title, uint32_t x, uint32_t y, uint32_t w, uint32_t h);
void studiolens_render_workspace(const char *workspace_name);

// Triconjectural command routing
void studiolens_execute_command(const char *args);

#endif // STUDIOLENS_H
