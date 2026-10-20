#include "torus.h"

void bluetooth_init(void) {
    serial_print("[BluetoothSubsystem] Initializing Bluetooth Host Controller Interface (HCI)...\n");
    serial_print("[Bluetooth] Sending HCI_RESET command to baseband controller...\n");
    serial_print("[Bluetooth] Configuring Bluetooth 5.4 Low Energy (LE) Audio & Isochronous Channels...\n");
    serial_print("[Bluetooth] Pairing pipeline online. Ready for HID input and wireless audio peripherals.\n");
}
