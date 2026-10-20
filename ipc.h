#ifndef IPC_H
#define IPC_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_IPC_ENDPOINTS 32
#define IPC_MSG_MAX_WORDS 16

typedef enum {
    IPC_PERM_READ = 1 << 0,
    IPC_PERM_WRITE = 1 << 1,
    IPC_PERM_EXEC = 1 << 2
} ipc_rights_t;

typedef struct {
    uint32_t sender_pid;
    uint32_t receiver_pid;
    uint32_t msg_tag;
    uint32_t payload[IPC_MSG_MAX_WORDS];
    uint32_t payload_size;
} ipc_message_t;

typedef struct {
    uint32_t endpoint_id;
    uint32_t owner_pid;
    uint32_t rights;
    ipc_message_t queue[8];
    uint32_t head;
    uint32_t tail;
    bool active;
} ipc_endpoint_t;

void ipc_subsystem_init(void);
uint32_t ipc_create_endpoint(uint32_t pid, uint32_t rights);
bool ipc_send(uint32_t dest_endpoint, ipc_message_t *msg);
bool ipc_receive(uint32_t endpoint_id, ipc_message_t *out_msg);

#endif // IPC_H
