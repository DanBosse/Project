#include <stdint.h>
#include <stddef.h>

#define MAX_EVENT_WATCHERS 64

typedef enum {
    EVENT_TYPE_READ = 1 << 0,
    EVENT_TYPE_WRITE = 1 << 1,
    EVENT_TYPE_TIMER = 1 << 2,
    EVENT_TYPE_ERROR = 1 << 3
} event_type_t;

typedef void (*event_callback_t)(uint32_t watcher_id, event_type_t event, void *user_data);

typedef struct {
    uint32_t watcher_id;
    int fd_or_channel;
    uint32_t events_mask;
    event_callback_t callback;
    void *user_data;
    uint8_t active;
} event_watcher_t;

static event_watcher_t reactor_watchers[MAX_EVENT_WATCHERS];
static uint32_t next_watcher_id = 10000;

void async_reactor_init(void) {
    for (int i = 0; i < MAX_EVENT_WATCHERS; i++) {
        reactor_watchers[i].active = 0;
    }
}

int async_reactor_register(int channel, uint32_t events, event_callback_t cb, void *user_data, uint32_t *out_watcher_id) {
    if (!cb || !out_watcher_id) return -1;

    for (int i = 0; i < MAX_EVENT_WATCHERS; i++) {
        if (!reactor_watchers[i].active) {
            reactor_watchers[i].watcher_id = next_watcher_id++;
            reactor_watchers[i].fd_or_channel = channel;
            reactor_watchers[i].events_mask = events;
            reactor_watchers[i].callback = cb;
            reactor_watchers[i].user_data = user_data;
            reactor_watchers[i].active = 1;

            *out_watcher_id = reactor_watchers[i].watcher_id;
            return 0;
        }
    }
    return -2; // Event watcher table full
}

int async_reactor_poll_once(uint32_t simulated_trigger_id) {
    int dispatched_count = 0;
    for (int i = 0; i < MAX_EVENT_WATCHERS; i++) {
        if (reactor_watchers[i].active) {
            // Simulate firing active event watchers for testing kernel event loop
            if (reactor_watchers[i].watcher_id == simulated_trigger_id || simulated_trigger_id == 0) {
                if (reactor_watchers[i].callback) {
                    reactor_watchers[i].callback(
                        reactor_watchers[i].watcher_id, 
                        (event_type_t)(reactor_watchers[i].events_mask & 0x0F), 
                        reactor_watchers[i].user_data
                    );
                    dispatched_count++;
                }
            }
        }
    }
    return dispatched_count;
}

int async_reactor_unregister(uint32_t watcher_id) {
    for (int i = 0; i < MAX_EVENT_WATCHERS; i++) {
        if (reactor_watchers[i].active && reactor_watchers[i].watcher_id == watcher_id) {
            reactor_watchers[i].active = 0;
            return 0;
        }
    }
    return -3; // Watcher ID not found
}
