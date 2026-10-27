#include <stdint.h>
#include <stddef.h>

#define MAX_BT_DEVICES 8
#define MAC_ADDR_LEN 6

typedef struct {
    uint8_t mac[MAC_ADDR_LEN];
    int rssi;
    uint8_t connected;
    uint8_t paired;
} bluetooth_device_t;

typedef struct {
    bluetooth_device_t devices[MAX_BT_DEVICES];
    uint32_t device_count;
    uint8_t scanning_active;
} bluetooth_subsystem_t;

static bluetooth_subsystem_t bt_manager;

void bluetooth_init(void) {
    bt_manager.device_count = 0;
    bt_manager.scanning_active = 0;
    for (int i = 0; i < MAX_BT_DEVICES; i++) {
        bt_manager.devices[i].connected = 0;
        bt_manager.devices[i].paired = 0;
    }
}

int bluetooth_register_device(const uint8_t *mac, int rssi) {
    if (!mac || bt_manager.device_count >= MAX_BT_DEVICES) return -1;
    
    bluetooth_device_t *dev = &bt_manager.devices[bt_manager.device_count];
    for (int i = 0; i < MAC_ADDR_LEN; i++) {
        dev->mac[i] = mac[i];
    }
    dev->rssi = rssi;
    dev->connected = 0;
    dev->paired = 0;
    
    bt_manager.device_count++;
    return 0;
}
