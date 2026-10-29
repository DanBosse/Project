// ============================================================================
// TorusOS Master State-of-the-Art GUI Desktop Environment
// Written entirely in AuraScript | Executing in Ring 3 Sandbox
// ============================================================================

import aura.color.oklch as oklch
import aura.ui.spatial as spatial
import aura.neural as neural

public class TorusDesktopWorkspace extends StudioWorkspace {
    private UserTerminal* main_terminal;
    private StudioLensDashboard* studiolens_window;
    private float background_luminance;

    public override def on_mount() {
        console.log("Booting TorusOS GUI Sandbox Environment...");
        
        // 1. Initialize per-pixel OKLCH color-accurate background
        this.background_luminance = 0.12; // Deep studio slate
        this.set_theme_color(oklch.from_values(this.background_luminance, 0.02, 240.0));

        // 2. Spawn the Core UserTerminal Node inside a spatial floating window
        this.main_terminal = spatial.spawn_window(UserTerminal, {
            title: "TorusOS Secure Terminal",
            width: 960,
            height: 600,
            x: 120,
            y: 80
        });

        // 3. Spawn the StudioLens Real-Time Telemetry Dashboard
        this.studiolens_window = spatial.spawn_window(StudioLensDashboard, {
            title: "StudioLens Diagnostics",
            width: 1024,
            height: 680,
            x: 1100,
            y: 80
        });

        // 4. Attach spring physics for buttery-smooth opening animation
        this.animate_workspace_entrance();
    }

    private async def animate_workspace_entrance() {
        let spring = create_spring({ stiffness: 140.0, damping: 15.0, mass: 1.0 });
        
        spring.on_update((scale_val: float) => {
            this.main_terminal.set_scale(scale_val);
            this.main_terminal.set_opacity(scale_val);
            this.studiolens_window.set_scale(scale_val);
            this.studiolens_window.set_opacity(scale_val);
        });

        await spring.start_animation(0.0, 1.0);
        console.log("TorusOS GUI Workspace fully interactive.");
    }
}

node UserTerminal extends StudioWidget {
    public layout: LayoutType = LayoutType.VERTICAL_STACK;
    public prompt: string = "user@torus-sandbox:/# ";
    public allow_ipc: bool = true;

    private terminal_buffer: list = [];
    private sandbox_ref: SandboxContext*;

    public override def on_init() {
        this.sandbox_ref = sandbox_create_context(SANDBOX_LEVEL_STRICT);
        if (this.allow_ipc) {
            ipc_enable_channel(this.sandbox_ref, IPC_CHANNEL_USER_TERMINAL);
        }
    }

    public def render() {
        this.begin_stack();
        this.draw_output_buffer(this.terminal_buffer);
        this.draw_prompt_line(this.prompt);
        this.end_stack();
    }
}
