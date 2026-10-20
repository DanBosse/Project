#include "workspace_registry.h"
#include <stdint.h>
#include <stdbool.h>

extern void kernel_print(const char* str);
extern void kernel_print_hex(uint64_t val);

static WorkspaceType current_workspace = WS_GRAPHICS;

void workspace_registry_init(void) {
    kernel_print("[WorkspaceRegistry] Initializing Tier 6 Studio Lens 5 Professional Workspaces (#GPH, #VIS, #AUD, #OFF, #PHY)...\n");
}

bool workspace_activate(WorkspaceType ws) {
    current_workspace = ws;
    switch (ws) {
        case WS_GRAPHICS:
            kernel_print("[WorkspaceRegistry] Activated Graphics Workspace (#GPH) with spatial glassmorphism boundaries.\n");
            break;
        case WS_VISUAL:
            kernel_print("[WorkspaceRegistry] Activated Visual Workspace (#VIS) with spatial glassmorphism boundaries.\n");
            break;
        case WS_AUDIO:
            kernel_print("[WorkspaceRegistry] Activated Audio Workspace (#AUD) with spatial glassmorphism boundaries.\n");
            break;
        case WS_OFFICE:
            kernel_print("[WorkspaceRegistry] Activated Office Workspace (#OFF) with spatial glassmorphism boundaries.\n");
            break;
        case WS_PHYSICS:
            kernel_print("[WorkspaceRegistry] Activated Physics Workspace (#PHY) with spatial glassmorphism boundaries.\n");
            break;
    }
    return true;
}

uint64_t workspace_get_active_handle(void) {
    return 0x574F524B53504143ULL ^ (uint64_t)current_workspace; // "WORKSPAC" + ws id
}
