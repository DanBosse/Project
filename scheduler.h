#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_THREADS 8

typedef enum {
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_TERMINATED
} thread_state_t;

typedef struct {
    uint32_t id;
    thread_state_t state;
    uint32_t esp;
    uint32_t stack_base;
} thread_t;

void scheduler_init(void);
int scheduler_create_thread(void (*entry_point)(void));
void scheduler_yield(void);

#endif // SCHEDULER_H
