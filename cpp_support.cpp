#include <stdint.h>
#include <stddef.h>

extern "C" {
    // 1. Handle pure virtual function calls
    void __cxa_pure_virtual(void) {
        while (1);
    }

    // 2. Global constructor execution hooks
    typedef void (*constructor_func)(void);
    extern constructor_func __init_array_start[];
    extern constructor_func __init_array_end[];

    void call_constructors(void) {
        for (constructor_func* func = __init_array_start; func < __init_array_end; func++) {
            (*func)();
        }
    }
}

// 3. Bare-metal memory allocation hooks for 'new' and 'delete'
void* operator new(size_t size) {
    (void)size;
    return nullptr;
}

void* operator new[](size_t size) {
    return operator new(size);
}

void operator delete(void* ptr) noexcept {
    (void)ptr;
}

void operator delete[](void* ptr) noexcept {
    operator delete(ptr);
}

void operator delete(void* ptr, size_t size) noexcept {
    (void)size;
    operator delete(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size;
    operator delete(ptr);
}
