#ifndef TORUS_H
#define TORUS_H

typedef unsigned int uint32_t;

void serial_print(const char* str);
void vmm_init(void);
void ipc_init(void);
void scheduler_init(void);
void vfs_init(void);
void smp_init(void);
void comms_init(void);
void server_daemon_init(void);
void physics_cad_init(void);

#endif
