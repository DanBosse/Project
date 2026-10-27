#include <stdint.h>
#include <stdbool.h>
#include "vfs.h"

bool vfs_mount(const char *path, int (*read_fn)(void*, uint32_t), int (*write_fn)(const void*, uint32_t)) {
    (void)path; (void)read_fn; (void)write_fn;
    return true;
}

bool vfs_read_file(const char *path, void *buf, uint32_t size) {
    (void)path; (void)buf; (void)size;
    return false;
}

int vfs_open(const char *path, int flags) {
    (void)path; (void)flags;
    return 1; // Dummy FD
}

ssize_t vfs_read(int fd, void *buf, size_t count) {
    (void)fd; (void)buf; (void)count;
    return 0; // EOF
}

int vfs_close(int fd) {
    (void)fd;
    return 0;
}
