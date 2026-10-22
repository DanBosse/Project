#include "aurascript.h"
#include "torus.h"
#include "spatial_compositor.h"
#include <stddef.h>

// Simple freestanding substring search helper
static int kernel_contains(const char* str, const char* substr) {
    if (!str || !substr) return 0;
    for (int i = 0; str[i] != '\0'; i++) {
        int j = 0;
        while (substr[j] != '\0' && str[i + j] == substr[j]) {
            j++;
        }
        if (substr[j] == '\0') return 1;
    }
    return 0;
}

void aurascript_eval(const char* script) {
    serial_print("[AuraScript] Evaluating script snippet...\n");
    if (script != NULL) {
        if (kernel_contains(script, "GRID") || kernel_contains(script, "grid")) {
            serial_print("[AuraScript FFI] Triggering layout change -> GRID\n");
        } else if (kernel_contains(script, "spawn")) {
            serial_print("[AuraScript FFI] Triggering window spawn command\n");
        }
    }
}
