#include <stdint.h>
#include <stddef.h>

#define MAX_USB_DEVICES 8

typedef struct {
    uint16_t vendor_id;
    uint16_t product_id;
    uint8_t endpoint_count;
    uint8_t connected;
} usb_device_t;

static usb_device_t usb_bus[MAX_USB_DEVICES];

void usb_init(void) {
    for (int i = 0; i < MAX_USB_DEVICES; i++) {
        usb_bus[i].vendor_id = 0;
        usb_bus[i].product_id = 0;
        usb_bus[i].endpoint_count = 0;
        usb_bus[i].connected = 0;
    }
}

int usb_register_device(uint16_t vid, uint16_t pid, uint8_t endpoints) {
    for (int i = 0; i < MAX_USB_DEVICES; i++) {
        if (!usb_bus[i].connected) {
            usb_bus[i].vendor_id = vid;
            usb_bus[i].product_id = pid;
            usb_bus[i].endpoint_count = endpoints;
            usb_bus[i].connected = 1;
            return i;
        }
    }
    return -1;
}
