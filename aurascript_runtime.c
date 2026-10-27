#include <stdint.h>
#include <stddef.h>

#define MAX_AURASCRIPT_JOBS 16
#define SCRIPT_BYTECODE_LEN 128

typedef enum {
    OP_NOP = 0,
    OP_ALLOC_VM,
    OP_DISPATCH_AI,
    OP_SYNC_CLUSTER,
    OP_HALT
} aurascript_opcode_t;

typedef struct {
    uint32_t script_id;
    uint8_t bytecode[SCRIPT_BYTECODE_LEN];
    size_t bytecode_length;
    uint32_t instruction_pointer;
    uint32_t execution_status; // 0 = Ready, 1 = Running, 2 = Completed, -1 = Error
    uint64_t virtual_registers[8];
    uint8_t active;
} aurascript_job_t;

static aurascript_job_t aurascript_registry[MAX_AURASCRIPT_JOBS];
static uint32_t next_script_id = 100000;

void aurascript_runtime_init(void) {
    for (int i = 0; i < MAX_AURASCRIPT_JOBS; i++) {
        aurascript_registry[i].active = 0;
        aurascript_registry[i].execution_status = 0;
    }
}

int aurascript_load_script(const uint8_t *code, size_t size, uint32_t *out_script_id) {
    if (!code || size == 0 || size > SCRIPT_BYTECODE_LEN || !out_script_id) return -1;

    for (int i = 0; i < MAX_AURASCRIPT_JOBS; i++) {
        if (!aurascript_registry[i].active) {
            aurascript_job_t *job = &aurascript_registry[i];
            job->script_id = next_script_id++;
            job->bytecode_length = size;
            job->instruction_pointer = 0;
            job->execution_status = 0; // Ready

            for (size_t j = 0; j < size; j++) {
                job->bytecode[j] = code[j];
            }
            for (int j = 0; j < 8; j++) {
                job->virtual_registers[j] = 0;
            }

            job->active = 1;
            *out_script_id = job->script_id;
            return 0;
        }
    }
    return -2; // AuraScript job registry full
}

int aurascript_step_execution(uint32_t script_id) {
    for (int i = 0; i < MAX_AURASCRIPT_JOBS; i++) {
        if (aurascript_registry[i].active && aurascript_registry[i].script_id == script_id) {
            aurascript_job_t *job = &aurascript_registry[i];

            if (job->execution_status == 2 || job->execution_status < 0) {
                return -3; // Script already finished or faulted
            }

            job->execution_status = 1; // Running

            if (job->instruction_pointer >= job->bytecode_length) {
                job->execution_status = 2; // Completed
                return 0;
            }

            uint8_t opcode = job->bytecode[job->instruction_pointer];
            job->instruction_pointer++;

            switch (opcode) {
                case OP_NOP:
                    break;
                case OP_ALLOC_VM:
                    job->virtual_registers[0] = 0x4000; // Simulated allocated VMID
                    break;
                case OP_DISPATCH_AI:
                    job->virtual_registers[1] = 1; // AI Acceleration success flag
                    break;
                case OP_SYNC_CLUSTER:
                    job->virtual_registers[2] = 0x7A4F8821; // Cluster consensus ACK
                    break;
                case OP_HALT:
                    job->execution_status = 2; // Completed
                    return 0;
                default:
                    job->execution_status = -1; // Unknown opcode error
                    return -4;
            }
            return 0;
        }
    }
    return -5; // Script ID not found
}
