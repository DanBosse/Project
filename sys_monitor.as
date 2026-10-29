import aura.color.oklch as oklch
import aura.sys.hardware as hw
import aura.ui.spatial as spatial

struct MetricBuffer {
    uint32_t cpu_usage_pct;
    uint32_t ram_used_mb;
    uint32_t ram_total_mb;
    uint64_t timestamp;
};

node SystemMonitorWidget extends StudioWidget {
    public layout: LayoutType = LayoutType.VERTICAL_STACK;
    public title: string = "TorusOS Performance Monitor";
    
    private metrics: MetricBuffer*;
    private is_updating: bool = false;
    private primary_color: object;

    public override def on_init() {
        this.metrics = alloc(sizeof(MetricBuffer));
        this.primary_color = oklch.from_values(0.68, 0.18, 220.0);
        this.set_border_color(this.primary_color);
        this.start_telemetry_loop();
    }

    private async def start_telemetry_loop() {
        this.is_updating = true;
        while (this.is_updating) {
            this.metrics->cpu_usage_pct = hw.get_cpu_usage();
            this.metrics->ram_used_mb = hw.get_used_ram_mb();
            this.metrics->ram_total_mb = hw.get_total_ram_mb();
            this.metrics->timestamp = hw.get_system_time();
            this.mark_dirty();
            await sleep(500);
        }
    }

    public def render() {
        this.begin_stack();
        let cpu_str = "CPU Load: " + string(this.metrics->cpu_usage_pct) + "%";
        this.draw_text(cpu_str, { x: 10, y: 15, color: this.primary_color });
        this.draw_progress_bar(this.metrics->cpu_usage_pct / 100.0, { x: 10, y: 35, width: 260 });

        let ram_str = "RAM: " + string(this.metrics->ram_used_mb) + " MB / " + string(this.metrics->ram_total_mb) + " MB";
        this.draw_text(ram_str, { x: 10, y: 65, color: oklch.from_values(0.80, 0.10, 100.0) });
        this.end_stack();
    }

    public override def on_destroy() {
        this.is_updating = false;
        free(this.metrics);
        console.log("SystemMonitorWidget container cleaned up successfully.");
    }
}
