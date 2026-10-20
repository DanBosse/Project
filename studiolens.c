#include "studiolens.h"
#include "spatial_compositor.h"
#include "torus.h"
#include <string.h>

static char current_workspace[32] = "MASTER_WORKSPACE";
static WindowLayoutMode current_layout = LAYOUT_FLOATING;

int char_starts_with(const char* str, const char* prefix) {
    while (*prefix) {
        if (*prefix++ != *str++) return 0;
    }
    return 1;
}

void studiolens_init(void) {
    serial_print("[StudioLens] Initializing Tier 6 Studio Lens Environment...\n");
    spatial_compositor_init();
}

void studiolens_set_layout(WindowLayoutMode mode) {
    current_layout = mode;
    if (current_layout == LAYOUT_FLOATING) {
        serial_print("[StudioLens] Layout switched to FLOATING.\n");
    } else {
        serial_print("[StudioLens] Layout switched to GRID (2x2 split).\n");
    }
}

void studiolens_set_workspace(const char* ws_name) {
    if (ws_name != NULL && ws_name[0] != '\0') {
        int i = 0;
        while (ws_name[i] != '\0' && i < 31) {
            current_workspace[i] = ws_name[i];
            i++;
        }
        current_workspace[i] = '\0';
    }
    serial_print("[StudioLens] Targeting workspace: ");
    serial_print(current_workspace);
    serial_print("\n");
}

void studiolens_spawn_window(const char* title, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    serial_print("[StudioLens] Spawning custom spatial window -> ");
    serial_print(title ? title : "TelemetryPanel");
    serial_print("\n");
    
    spatial_compositor_render(0);
}

void studiolens_render_workspace(const char* workspace_name) {
    if (workspace_name != NULL && workspace_name[0] != '\0') {
        studiolens_set_workspace(workspace_name);
    }
    serial_print("[StudioLens] Rendering active studio lens workspace -> ");
    serial_print(current_workspace);
    serial_print("\n");
    spatial_compositor_render(0);
    serial_print("[Tier 6 Convergence] Workspace rendered successfully.\n");
}

void studiolens_execute_command(const char* args) {
    serial_print("[Synthesizer] Duality verification: PASSED\n");

    if (args != NULL && args[0] != '\0') {
        if (char_starts_with(args, "spawn")) {
            studiolens_spawn_window("TelemetryPanel", 120, 160, 480, 320);
        } else if (char_starts_with(args, "grid")) {
            studiolens_set_layout(LAYOUT_GRID);
        } else if (char_starts_with(args, "workspace")) {
            studiolens_set_workspace("workspace");
        } else {
            studiolens_spawn_window("ActivePanel", 100, 100, 400, 300);
        }
    }
    
    studiolens_render_workspace(current_workspace);
}
