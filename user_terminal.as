// user_terminal.as - Declarative Sandbox UI Node
node UserTerminal extends StudioWidget {
    public layout: LayoutType = LayoutType.VERTICAL_STACK;
    public prompt: string = "user@torus-sandbox:/# ";
    public allow_ipc: bool = true;

    private terminal_buffer: list = [];
    private sandbox_ref: SandboxContext*;

    public override def on_init() {
        // Attach this terminal directly to the Ring 3 Sandbox Context
        this.sandbox_ref = sandbox_create_context(SANDBOX_LEVEL_STRICT);
        
        if (this.allow_ipc) {
            ipc_enable_channel(this.sandbox_ref, IPC_CHANNEL_USER_TERMINAL);
        }

        console.log("UserTerminal initialized inside GUI Sandbox.");
    }

    public def print_prompt() {
        this.write_output(this.prompt);
    }
}
