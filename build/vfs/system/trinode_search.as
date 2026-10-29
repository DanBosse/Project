// ============================================================================
// Triconjectural Search Bar Workspace Overlay
// Written in AuraScript | Ring 3 Sandbox
// ============================================================================

import aura.ui.spatial as spatial
import aura.triconjectural as tricon

public class TrinodeSearchBar extends StudioOverlay {
    private input_query: string = "";
    private results_list: list = [];

    public override def on_mount() {
        console.log("Triconjectural Search Overlay mounted.");
    }

    public def on_search_input(query: string) {
        this.input_query = query;
        // Query the kernel Triconjectural engine across OS states, data, and net
        this.results_list = tricon.query_all(query, { threshold: 0.80 });
        this.render_results();
    }

    private def render_results() {
        for (let res in this.results_list) {
            console.log(`[MATCH] ${res.path} (Score: ${res.probability})`);
        }
    }
}
