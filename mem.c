#include <stddef.h>
#include <stdint.h>

static uint8_t heap_pool[1024 * 1024]; // 1MB static kernel heap pool
static size_t heap_offset = 0;

void* kmalloc_aligned(size_t size, size_t alignment) {
    size_t current_addr = (size_t)(heap_pool + heap_offset);
    size_t remainder = current_addr % alignment;
    if (remainder != 0) {
        heap_offset += (alignment - remainder);
    }
    
    if (heap_offset + size > sizeof(heap_pool)) {
        return NULL; // Out of memory
    }

    void* ptr = (void*)(heap_pool + heap_offset);
    heap_offset += size;
    return ptr;
}

void kfree(void* ptr) {
    (void)ptr; // Bump allocator stub
}
