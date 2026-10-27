#include <stdint.h>
#include <stddef.h>

#define MAX_WORKFLOW_TASKS 32
#define MAX_NAME_LEN 32

typedef struct {
    char name[MAX_NAME_LEN];
    int (*task_func)(void);
    uint32_t dependencies[8];
    uint8_t dep_count;
    uint8_t executed;
} aurascript_workflow_task_t;

typedef struct {
    aurascript_workflow_task_t tasks[MAX_WORKFLOW_TASKS];
    uint32_t task_count;
} aurascript_wfs_registry_t;

static aurascript_wfs_registry_t global_wfs_registry;

void aurascript_wfs_init(void) {
    global_wfs_registry.task_count = 0;
    for (int i = 0; i < MAX_WORKFLOW_TASKS; i++) {
        global_wfs_registry.tasks[i].executed = 0;
        global_wfs_registry.tasks[i].dep_count = 0;
    }
}

int aurascript_wfs_register_task(const char *name, int (*func)(void)) {
    if (!name || !func || global_wfs_registry.task_count >= MAX_WORKFLOW_TASKS) {
        return -1;
    }
    
    uint32_t idx = global_wfs_registry.task_count++;
    aurascript_workflow_task_t *task = &global_wfs_registry.tasks[idx];
    
    // Copy name safely
    int i = 0;
    while (name[i] != '\0' && i < MAX_NAME_LEN - 1) {
        task->name[i] = name[i];
        i++;
    }
    task->name[i] = '\0';
    
    task->task_func = func;
    task->executed = 0;
    return (int)idx;
}

int aurascript_wfs_execute_workflow(void) {
    int success_count = 0;
    for (uint32_t i = 0; i < global_wfs_registry.task_count; i++) {
        aurascript_workflow_task_t *task = &global_wfs_registry.tasks[i];
        if (!task->executed && task->task_func) {
            if (task->task_func() == 0) {
                task->executed = 1;
                success_count++;
            } else {
                return -1; // Workflow halted on failure
            }
        }
    }
    return success_count;
}
