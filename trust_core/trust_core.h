#ifndef TRUST_CORE_H
#define TRUST_CORE_H

#include <stdint.h>

#define CONTAINER_PHYSICS_LAB 0x0B
#define TELEMETRY_TYPE_SPATIAL_TENSOR 1
#define RING0_ZERO_COPY_BUFFER 0

// FFI bindings and subsystem declarations
void trust_physics_update_vector(int32_t px, int32_t py, int32_t pz);
int32_t trust_physics_get_velocity_z(void);
void trust_core_verify_sandbox(int container_id);
void studiolens_register_stream(int id, const char* name, int type, int flags);
void studiolens_emit_log(const char* msg);

#endif // TRUST_CORE_H
