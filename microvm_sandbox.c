#include <stdint.h>
#include <stddef.h>

#define MAX_MICROVMS 16
#define MICROVM_NAME_LEN 32

typedef enum {
    VM_STATE_CREATED = 0,
    VM_STATE_RUNNING,
    VM_STATE_SUSPENDED,
    VM_STATE_TERMINATED
} microvm_state_t;

typedef struct {
    uint32_t vmid;
    char name[MICROVM_NAME_LEN];
    uint32_t tier_level;
    uint64_t memory_base;
    uint64_t memory_size;
    uint64_t vcpu_rip;
    uint64_t vcpu_rsp;
    microvm_state_t state;
    uint8_t active;
} microvm_enclave_t;

static microvm_enclave_t microvm_registry[MAX_MICROVMS];
static uint32_t next_vmid = 4000;

void microvm_sandbox_init(void) {
    for (int i = 0; i < MAX_MICROVMS; i++) {
        microvm_registry[i].active = 0;
        microvm_registry[i].state = VM_STATE_TERMINATED;
    }
}

int microvm_create_enclave(const char *name, uint32_t tier_level, uint64_t mem_size, uint32_t *out_vmid) {
    if (!name || mem_size == 0 || !out_vmid) return -1;

    for (int i = 0; i < MAX_MICROVMS; i++) {
        if (!microvm_registry[i].active) {
            microvm_enclave_t *vm = &microvm_registry[i];
            vm->vmid = next_vmid++;
            
            int j = 0;
            while (name[j] != '\0' && j < MICROVM_NAME_LEN - 1) {
                vm->name[j] = name[j];
                j++;
            }
            vm->name[j] = '\0';

            vm->tier_level = tier_level;
            vm->memory_base = 0x20000000ULL + (i * 0x1000000ULL); // Isolated virtual memory base
            vm->memory_size = mem_size;
            vm->vcpu_rip = 0x1000; // Default reset vector
            vm->vcpu_rsp = 0x90000;
            vm->state = VM_STATE_CREATED;
            vm->active = 1;

            *out_vmid = vm->vmid;
            return 0;
        }
    }
    return -2; // MicroVM registry full
}

int microvm_set_state(uint32_t vmid, microvm_state_t new_state) {
    for (int i = 0; i < MAX_MICROVMS; i++) {
        if (microvm_registry[i].active && microvm_registry[i].vmid == vmid) {
            microvm_registry[i].state = new_state;
            return 0;
        }
    }
    return -3; // VMID not found
}
