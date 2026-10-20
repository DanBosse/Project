#include "mm.h"

// Static kernel heap buffer (1MB)
static uint8_t kernel_heap[1024 * 1024];
static size_t heap_offset = 0;

void* kmalloc_aligned(size_t size, size_t alignment) {
    size_t current_addr = (size_t)(kernel_heap + heap_offset);
    size_t remainder = current_addr % alignment;
    if (remainder != 0) {
        heap_offset += (alignment - remainder);
    }
    
    if (heap_offset + size > sizeof(kernel_heap)) {
        return NULL; // Out of memory
    }
    
    void *ptr = (void *)(kernel_heap + heap_offset);
    heap_offset += size;
    return ptr;
}

void kfree(void* ptr) {
    // In a deterministic static bump allocator, individual frees are no-ops
    (void)ptr;
}
