#ifndef _SSIZE_T_DEFINED
#define _SSIZE_T_DEFINED
typedef long ssize_t;
#endif

#include <stdbool.h>
#include <stdint.h>

void trust_tensor_bounds_verify(void) {}
void trust_syscall_verify(void) {}
#include "sandbox.h"
bool trust_sandbox_verify(const SandboxInstruction* program, size_t length) { (void)program; (void)length; return true; }

int vfs_open(const char *path, int flags) { (void)path; (void)flags; return -1; }
ssize_t vfs_read(int fd, void *buf, size_t count) { (void)fd; (void)buf; (void)count; return -1; }
int vfs_close(int fd) { (void)fd; return -1; }

void terminal_print(const char *str) {
    (void)str;
}
