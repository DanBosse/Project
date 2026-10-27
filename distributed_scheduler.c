#include <stdint.h>
#include <stddef.h>

#define MAX_TASK_QUEUE 64

typedef enum {
    TARGET_EXEC_CPU = 0,
    TARGET_EXEC_GPU,
    TARGET_EXEC_NPU,
    TARGET_EXEC_DPU,
    TARGET_EXEC_FEDERATED_PEER
} execution_target_t;

typedef struct {
    uint32_t task_id;
    execution_target_t target;
    uint32_t priority;
    uint64_t data_payload;
    uint8_t status; // 0 = Queued, 1 = Dispatched, 2 = Completed, -1 = Failed
} scheduled_task_t;

typedef struct {
    scheduled_task_t queue[MAX_TASK_QUEUE];
    uint32_t head;
    uint32_t tail;
    uint32_t total_dispatched;
} task_scheduler_t;

static task_scheduler_t global_scheduler;
static uint32_t next_task_id = 10000;

void distributed_scheduler_init(void) {
    global_scheduler.head = 0;
    global_scheduler.tail = 0;
    global_scheduler.total_dispatched = 0;
    for (int i = 0; i < MAX_TASK_QUEUE; i++) {
        global_scheduler.queue[i].status = 0;
    }
}

int distributed_scheduler_submit(execution_target_t target, uint32_t priority, uint64_t payload, uint32_t *out_task_id) {
    if (!out_task_id) return -1;
    
    uint32_t next_head = (global_scheduler.head + 1) % MAX_TASK_QUEUE;
    if (next_head == global_scheduler.tail) {
        return -2; // Task queue full
    }
    
    scheduled_task_t *task = &global_scheduler.queue[global_scheduler.head];
    task->task_id = next_task_id++;
    task->target = target;
    task->priority = priority;
    task->data_payload = payload;
    task->status = 0; // Queued
    
    global_scheduler.head = next_head;
    *out_task_id = task->task_id;
    return 0;
}

int distributed_scheduler_dispatch_next(void) {
    if (global_scheduler.head == global_scheduler.tail) {
        return -3; // No tasks pending in queue
    }
    
    scheduled_task_t *task = &global_scheduler.queue[global_scheduler.tail];
    task->status = 1; // Dispatched to target hardware/peer
    global_scheduler.tail = (global_scheduler.tail + 1) % MAX_TASK_QUEUE;
    global_scheduler.total_dispatched++;
    
    return (int)task->task_id;
}
