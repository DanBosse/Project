#ifndef WORKSPACE_REGISTRY_H
#define WORKSPACE_REGISTRY_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    WS_GRAPHICS = 0, // #GPH
    WS_VISUAL,       // #VIS
    WS_AUDIO,        // #AUD
    WS_OFFICE,       // #OFF
    WS_PHYSICS       // #PHY
} WorkspaceType;

void workspace_registry_init(void);
bool workspace_activate(WorkspaceType ws);
uint64_t workspace_get_active_handle(void);

#endif
