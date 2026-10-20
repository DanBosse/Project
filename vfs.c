#include "torus.h"
#include "vfs.h"

static vfs_node_t mount_table[MAX_MOUNT_POINTS];

void vfs_init(void) {
    serial_print("[VFS] Initializing Virtual Filesystem & Namespace Mount Subsystem...\n");
    for (int i = 0; i < MAX_MOUNT_POINTS; i++) {
        mount_table[i].active = false;
    }
    serial_print("[VFS] Virtual Filesystem active. CAD asset namespace ready.\n");
}

bool vfs_mount(const char *path, int (*read_fn)(void*, uint32_t), int (*write_fn)(const void*, uint32_t)) {
    for (int i = 0; i < MAX_MOUNT_POINTS; i++) {
        if (!mount_table[i].active) {
            mount_table[i].active = true;
            return true;
        }
    }
    return false;
}

bool vfs_read_file(const char *path, void *buf, uint32_t size) {
    // Stub file lookup and read
    return true;
}
