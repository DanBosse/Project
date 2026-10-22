#include "studiolens.h"
#include "spatial_compositor.h"
#include "aurascript.h"
#include "torus.h"
#include <string.h>

static char current_workspace[32] = "TRICONJECTURAL_STUDIO";
static WindowLayoutMode current_layout = LAYOUT_GRID; // 2x2 layout perfect for tripartite + display

int char_starts_with_studio(const char* str, const char* prefix) {
    while (*prefix) {
        if (*prefix++ != *str++) return 0;
    }
    return 1;
}

void studiolens_init(void) {
    serial_print("[StudioLens] Initializing Tripartite StudioLens Environment...\n");
    spatial_compositor_init();
    aurascript_init();
}

void studiolens_set_layout(WindowLayoutMode mode) {
    current_layout = mode;
    if (current_layout == LAYOUT_FLOATING) {
        serial_print("[StudioLens] Layout switched to FLOATING.\n");
    } else {
        serial_print("[StudioLens] Layout switched to GRID (Tripartite Workspace Split).\n");
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
    (void)x; (void)y; (void)w; (void)h;
    serial_print("[StudioLens] Spawning tripartite panel -> ");
    serial_print(title ? title : "SearchPanel");
    serial_print("\n");
    spatial_compositor_render(current_layout);
}

void studiolens_render_workspace(const char* workspace_name) {
    if (workspace_name != NULL && workspace_name[0] != '\0') {
        studiolens_set_workspace(workspace_name);
    }
    serial_print("[StudioLens] Rendering active Tripartite Search Workspace...\n");
    spatial_compositor_render(current_layout);
    serial_print("[Tripartite Convergence] Data Files | Internet | OS panels active.\n");
}

void studiolens_execute_command(const char* args) {
    serial_print("[Triconjectural Engine] Query execution dispatched...\n");

    if (args != NULL && args[0] != '\0') {
        // Route query based on active search bar target or fallback to general eval
        if (char_starts_with_studio(args, "search:data ")) {
            serial_print("[Search Bar 1: Data Files] Query: ");
            serial_print(args + 12);
            serial_print("\n");
        } else if (char_starts_with_studio(args, "search:net ")) {
            serial_print("[Search Bar 2: Internet] Query: ");
            serial_print(args + 11);
            serial_print("\n");
        } else if (char_starts_with_studio(args, "search:os ")) {
            serial_print("[Search Bar 3: OS] Query: ");
            serial_print(args + 10);
            serial_print("\n");
        } else {
            // Default fallback to AuraScript evaluator
            aurascript_eval(args);
        }
    }

    studiolens_render_workspace(current_workspace);
}

#include "enterprise_telemetry.h"

void studiolens_telemetry_bridge_init(void) {
    enterprise_telemetry_dump();
    studiolens_render_workspace("triconjectural_search");
}
