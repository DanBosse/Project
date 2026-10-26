#include <stddef.h>
#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef long ssize_t;
#endif
#ifndef VFS_H
#define VFS_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_MOUNT_POINTS 8
#define MAX_FILENAME 32

typedef struct {
    char name[MAX_FILENAME];
    int (*read)(void *buf, uint32_t size);
    int (*write)(const void *buf, uint32_t size);
    bool active;
} vfs_node_t;

void vfs_init(void);
bool vfs_mount(const char *path, int (*read_fn)(void*, uint32_t), int (*write_fn)(const void*, uint32_t));
bool vfs_read_file(const char *path, void *buf, uint32_t size);

#endif // VFS_H


int vfs_open(const char *path, int flags);
ssize_t vfs_read(int fd, void *buf, size_t count);
int vfs_close(int fd);
