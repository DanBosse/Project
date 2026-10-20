#ifndef MM_H
#define MM_H

#include <stddef.h>
#include <stdint.h>

void* kmalloc_aligned(size_t size, size_t alignment);
void kfree(void* ptr);

#endif
