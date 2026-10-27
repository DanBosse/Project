#include <stdint.h>
#include <stddef.h>

#define MAX_MICROVMS 8
#define VM_MEMORY_SIZE (16 * 1024 * 1024) // 16MB per microVM sandbox

typedef struct {
    uint32_t vmid;
    uint32_t vcpu_count;
    uint64_t instruction_pointer;
    uint8_t memory_pool[VM_MEMORY_SIZE];
    size_t memory_size;
    uint8_t running;
} microvm_context_t;

static microvm_context_t microvm_registry[MAX_MICROVMS];

void microvm_manager_init(void) {
    for (int i = 0; i < MAX_MICROVMS; i++) {
        microvm_registry[i].running = 0;
        microvm_registry[i].vmid = i + 100;
        microvm_registry[i].memory_size = VM_MEMORY_SIZE;
    }
}

int microvm_create_instance(uint32_t vcpu_count, uint64_t entry_rip) {
    for (int i = 0; i < MAX_MICROVMS; i++) {
        if (!microvm_registry[i].running) {
            microvm_registry[i].vcpu_count = vcpu_count;
            microvm_registry[i].instruction_pointer = entry_rip;
            microvm_registry[i].running = 1;
            return (int)microvm_registry[i].vmid;
        }
    }
    return -1; // Max microVM limit reached
}

int microvm_terminate_instance(uint32_t vmid) {
    for (int i = 0; i < MAX_MICROVMS; i++) {
        if (microvm_registry[i].running && microvm_registry[i].vmid == vmid) {
            microvm_registry[i].running = 0;
            return 0;
        }
    }
    return -2; // VMID not found
}
