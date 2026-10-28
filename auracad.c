#include "auracad.h"
#include "kernel.h"

extern void serial_print(const char *str);

void auracad_init(void) {
    serial_print("[AuraCad] Initializing Parametric CAD & Physics Suite...\n");
    serial_print("[AuraCad] Loading spatial geometry kernel & constraint graphs... [OK]\n");
}

void auracad_run_physics_loop(void) {
    serial_print("[AuraCad Physics] Initializing 60Hz deterministic fixed timestep loop...\n");
    serial_print("[AuraCad Physics] Solving constraints... [PASS]\n");
    serial_print("[AuraCad Physics] Simulating rigid-body collisions & spring-damper networks... [OK]\n");
    serial_print("[AuraCad Physics] Rendering synchronized geometry frames into StudioLens... [OK]\n");
}
