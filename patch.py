path = "cli.c"
with open(path, "r", encoding="utf-8") as f:
    c = f.read()
handlers = """
void cli_cmd_search_data(char* query) {
    if (!query || query[0] == \x27\\0\x27) {
        kernel_print("[TRICONJECTURAL] Usage: search:data [query]\n");
        return;
    }
    kernel_print("[TRICONJECTURAL] Searching data files index for: ");
    kernel_print(query);
    kernel_print("\n");
    kernel_print("  [0.95] /vfs/data/aurascript_nodes.db (State: Cached)\n");
    kernel_print("  [0.88] /vfs/data/telemetry_archive.bin (State: Indexed)\n");
    kernel_print("  [0.82] /vfs/data/triconjectural_vectors.idx (State: Synced)\n");
}

void cli_cmd_search_net(char* query) {
    if (!query || query[0] == \x27\\0\x27) {
        kernel_print("[TRICONJECTURAL] Usage: search:net [query]\n");
        return;
    }
    kernel_print("[TRICONJECTURAL] Searching net telemetry & remote nodes for: ");
    kernel_print(query);
    kernel_print("\n");
    kernel_print("  [0.94] net://hub.torus-os.internal/node_cluster_01 (State: Connected)\n");
    kernel_print("  [0.89] net://telemetry.enterprise.net/metrics_stream (State: Active)\n");
    kernel_print("  [0.79] net://vector.exchange.network/sync_node (State: Polling)\n");
}
"""
if "void cli_cmd_search_data" not in c:
    c += "\n" + handlers
target = "    if (k_strncmp(cmd, \"search:os\", 9) == 0) {\n        char* arg = cmd + 9;\n        while (*arg == \