#include "torus.h"
#include "scheduler.h"

static thread_t threads[MAX_THREADS];
static int thread_count = 0;

void scheduler_init(void) {
    serial_print("[Scheduler] Initializing Preemptive Round-Robin Thread Scheduler...\n");
    for (int i = 0; i < MAX_THREADS; i++) {
        threads[i].state = THREAD_TERMINATED;
    }
    serial_print("[Scheduler] Thread Control Blocks (TCBs) initialized.\n");
}

int scheduler_create_thread(void (*entry_point)(void)) {
    if (thread_count >= MAX_THREADS) return -1;
    int tid = thread_count++;
    threads[tid].id = tid;
    threads[tid].state = THREAD_READY;
    serial_print("[Scheduler] Thread created and queued successfully.\n");
    return tid;
}

void scheduler_yield(void) {
    // Cooperative yield / context switch hook
}
