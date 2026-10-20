#include <stdint.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>

#define AURA_RING_CAPACITY 16

typedef enum {
    AURA_PIXEL_FORMAT_BGRA8888 = 0,
    AURA_PIXEL_FORMAT_RGBA8888,
    AURA_PIXEL_FORMAT_RGBA16F,
    AURA_PIXEL_FORMAT_RGB32F
} AuraPixelFormat;

typedef struct {
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    AuraPixelFormat format;
    uint64_t shm_handle;
    uintptr_t virtual_address;
} AuraSurfaceDescriptor;

typedef struct {
    uint64_t sequence_id;
    uint64_t timestamp_ns;
    uint32_t flags;
    AuraSurfaceDescriptor surface;
} AuraFrameNode;

typedef struct {
    atomic_size_t head __attribute__((aligned(64)));
    atomic_size_t tail __attribute__((aligned(64)));
    AuraFrameNode nodes[AURA_RING_CAPACITY];
} AuraZeroCopyRing;

bool aura_ring_enqueue(AuraZeroCopyRing *ring, const AuraFrameNode *node) {
    size_t head = atomic_load_explicit(&ring->head, memory_order_relaxed);
    size_t tail = atomic_load_explicit(&ring->tail, memory_order_acquire);
    if ((head - tail) >= AURA_RING_CAPACITY) return false;
    ring->nodes[head & (AURA_RING_CAPACITY - 1)] = *node;
    atomic_store_explicit(&ring->head, head + 1, memory_order_release);
    return true;
}

bool aura_ring_dequeue(AuraZeroCopyRing *ring, AuraFrameNode *node) {
    size_t tail = atomic_load_explicit(&ring->tail, memory_order_relaxed);
    size_t head = atomic_load_explicit(&ring->head, memory_order_acquire);
    if (tail == head) return false;
    *node = ring->nodes[tail & (AURA_RING_CAPACITY - 1)];
    atomic_store_explicit(&ring->tail, tail + 1, memory_order_release);
    return true;
}
