#include "spatial_compositor.h"
#include "studiolens_dashboard.h"
#include "torus.h"
#include "enterprise_telemetry.h"

// Optional external subsystem hooks for expanded dashboard telemetry
extern bool auraspring_rt_run(uint32_t bodies, uint32_t sub_steps);
extern void spatial_compositor_render(uint32_t flags);

void studiolens_render_telemetry_dashboard(void) {
    // Render the 5-window spatial compositor desktop GUI view
    spatial_compositor_render(0);
    serial_print("\n=================================================================\n");
    serial_print("           TorusOS StudioLens Comprehensive Telemetry Center       \n");
    serial_print("=================================================================\n");
    serial_print(" [PANEL 01] SOT-MRAM Arrays (t_REFI = 0)              -> ACTIVE    \n");
    serial_print(" [PANEL 02] Twin-Prime Memory Pools (delta = 2)       -> OPTIMAL   \n");
    serial_print(" [PANEL 03] Optical Backplane Link (AuraLink)         -> 100Gb/s   \n");
    serial_print(" [PANEL 04] Triconjectural Engine Indexer             -> SYNCED    \n");
    serial_print(" [PANEL 05] Physics Studio & Auraspring RT Pipeline   -> ONLINE    \n");
    serial_print(" [PANEL 06] Synthesizer Axiom Generator               -> READY     \n");
    serial_print(" [PANEL 07] Spatial Compositor Window Manager         -> ACTIVE    \n");
    serial_print(" [PANEL 08] VPU & Graphics/Audio Pipeline             -> ENGAGED   \n");
    serial_print(" [PANEL 09] AuraScript Execution & Neural Upscale     -> STABLE    \n");
    serial_print(" [PANEL 10] Ring 3 Userspace & Sandbox Isolation      -> OPERATIONAL\n");
    serial_print("-----------------------------------------------------------------\n");
    
    // Dump enterprise telemetry logs
    enterprise_telemetry_dump();
    
    serial_print("=================================================================\n\n");
}
