#ifndef IPC_H
#define IPC_H

#include <stdint.h>

#define MAX_TIERS 8
#define IPC_MSG_MAX_DATA 48

typedef struct {
    uint32_t sender_id;
    uint32_t receiver_id;
    uint32_t msg_type;
    uint32_t flags;
    uint8_t data[IPC_MSG_MAX_DATA];
} ipc_message_t;

void ipc_init(void);
int ipc_send(uint8_t tier_id, const ipc_message_t *msg);
int ipc_receive(uint8_t tier_id, ipc_message_t *msg);

#endif
