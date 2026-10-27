#include <stdint.h>
#include <stddef.h>

#define MAX_REACTOR_EVENTS 64

typedef enum {
    EVENT_TYPE_READ = 1,
    EVENT_TYPE_WRITE,
    EVENT_TYPE_TIMER,
    EVENT_TYPE_SIGNAL
} event_type_t;

typedef struct {
    uint32_t event_id;
    uint32_t fd_or_source;
    event_type_t type;
    uint32_t priority;
    uint8_t triggered;
    uint8_t active;
} reactor_event_t;

static reactor_event_t event_reactor_table[MAX_REACTOR_EVENTS];
static uint32_t next_reactor_event_id = 30000;

void event_reactor_init(void) {
    for (int i = 0; i < MAX_REACTOR_EVENTS; i++) {
        event_reactor_table[i].active = 0;
        event_reactor_table[i].triggered = 0;
    }
}

int event_reactor_register(uint32_t source, event_type_t type, uint32_t priority, uint32_t *out_event_id) {
    if (!out_event_id) return -1;

    for (int i = 0; i < MAX_REACTOR_EVENTS; i++) {
        if (!event_reactor_table[i].active) {
            reactor_event_t *ev = &event_reactor_table[i];
            ev->event_id = next_reactor_event_id++;
            ev->fd_or_source = source;
            ev->type = type;
            ev->priority = priority;
            ev->triggered = 0;
            ev->active = 1;

            *out_event_id = ev->event_id;
            return 0;
        }
    }
    return -2; // Event reactor table full
}

int event_reactor_poll_dispatch(uint32_t *out_dispatched_count) {
    if (!out_dispatched_count) return -1;

    uint32_t count = 0;
    for (int i = 0; i < MAX_REACTOR_EVENTS; i++) {
        if (event_reactor_table[i].active && !event_reactor_table[i].triggered) {
            // Simulate asynchronous event triggering and dispatch
            event_reactor_table[i].triggered = 1;
            count++;
        }
    }

    *out_dispatched_count = count;
    return 0;
}
