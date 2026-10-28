#include "trust_core.h"
#include <studiolens.h>

#define CONTAINER_PHYSICS_LAB 0x0B

void init_physics_telemetry(void) {
    // Lock Ring 0 memory boundary for Container 0x0B
    trust_core_verify_sandbox(CONTAINER_PHYSICS_LAB);

    // Bind AuraSpring spatial force field & frame timing to StudioLens pipe
    studiolens_register_stream(
        CONTAINER_PHYSICS_LAB,
        "AuraSpring_ForceField",
        TELEMETRY_TYPE_SPATIAL_TENSOR,
        RING0_ZERO_COPY_BUFFER
    );

    studiolens_emit_log("[TrustCore] Container 0x0B (PhysicsLab): Attested & Stream Linked.");
}
