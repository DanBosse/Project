#ifndef SPATIAL_COMPOSITOR_H
#define SPATIAL_COMPOSITOR_H

#include <stdint.h>

void spatial_compositor_init(void);
void spatial_compositor_render(uint32_t flags);
void spatial_compositor_feed_input(char c);
void spatial_compositor_update_scancode(uint8_t scancode);
void spatial_compositor_handle_char(char c);

#endif
