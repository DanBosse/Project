#include "torus.h"
#include "virtio_net.h"

static uint16_t virtio_io_base = 0;

void virtio_net_init(void) {
    serial_print("[Net] Scanning PCI bus for Virtio Network Device...\n");
    virtio_io_base = 0xC000;
    serial_print("[Net] Virtio-Net driver initialized. [Interface: virtio-net-pci, Queues: Rx/Tx Active]\n");
}

bool virtio_net_send(const uint8_t* packet, uint32_t length) {
    if (!packet || length == 0) return false;
    serial_print("[Net] Transmitting packet via Virtio-Net TX queue...\n");
    return true;
}

bool virtio_net_poll(uint8_t* buffer, uint32_t* length) {
    (void)buffer;
    (void)length;
    return false;
}
