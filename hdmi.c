#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint32_t pixel_clock;
    uint32_t width;
    uint32_t height;
    uint8_t audio_enabled;
    uint8_t connected;
} hdmi_display_t;

static hdmi_display_t primary_hdmi;

void hdmi_init(void) {
    primary_hdmi.pixel_clock = 148500; // 1080p60 standard clock
    primary_hdmi.width = 1920;
    primary_hdmi.height = 1080;
    primary_hdmi.audio_enabled = 1;
    primary_hdmi.connected = 1;
}

int hdmi_get_status(void) {
    return primary_hdmi.connected;
}

void hdmi_set_resolution(uint32_t w, uint32_t h) {
    primary_hdmi.width = w;
    primary_hdmi.height = h;
}
