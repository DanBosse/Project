#include <stdint.h>
#include <stddef.h>

extern void spatial_compositor_init(uint32_t *fb, uint32_t width, uint32_t height, uint32_t pitch);
extern void spatial_compositor_render_frame(void);


// Subsystem API declarations for testing
int boot_attestation_extend_pcr(uint32_t pcr_index, const uint8_t *data, size_t size);
int hsm_vault_generate_key(int type, uint32_t usage, uint32_t *out_id);
int capabilities_mint(uint32_t tier, uint32_t res_id, uint32_t rights, uint32_t *out_token);
int raft_append_log(const uint8_t *data, size_t size, uint32_t *out_idx);
int dfs_storage_store_block(uint32_t parent_id, const uint8_t *data, size_t size, uint32_t *out_id);
int cluster_scheduler_register_node(uint32_t node_id, uint32_t cores, uint64_t mem);
int accelerator_register_device(int type, uint32_t *out_id);
int ai_load_balancer_register_device(int type, uint32_t *out_id);
int microvm_create_enclave(const char *name, uint32_t tier, uint64_t mem, uint32_t *out_vmid);
int aurascript_load_script(const uint8_t *code, size_t size, uint32_t *out_id);
int zkp_provenance_submit(uint32_t tier, const uint8_t *stmt, size_t s_size, const uint8_t *proof, size_t p_size, uint32_t *out_id);
int compression_zip(const uint8_t *src, size_t src_len, uint8_t *dst, size_t *dst_len);
int live_migration_start(uint32_t vmid, uint32_t src, uint32_t target, uint32_t mem_bytes, uint32_t *out_sess);
int kernel_bypass_transmit(uint32_t dest, const uint8_t *data, size_t size, uint32_t *out_pkt);
int event_reactor_register(uint32_t source, int type, uint32_t priority, uint32_t *out_ev);

int run_torusos_subsystem_tests(void) {
    uint32_t id_out = 0;
    uint8_t scratch[128];
    size_t scratch_len = sizeof(scratch);

    // 1. Test Boot Attestation PCR Extension
    const uint8_t measure_payload[] = "TORUSOS_BOOT_MEASUREMENT";
    if (boot_attestation_extend_pcr(0, measure_payload, sizeof(measure_payload)) != 0) return -101;

    // 2. Test HSM Vault Key Generation
    if (hsm_vault_generate_key(1, 0x01 | 0x02, &id_out) != 0) return -102;

    // 3. Test Capability Token Minting (Tier 1 Sovereign Access)
    if (capabilities_mint(1, 0x5000, 0x07, &id_out) != 0) return -103;

    // 4. Test Cluster Node Registration & Job Scheduling
    if (cluster_scheduler_register_node(101, 32, 65536) != 0) return -104;

    // 5. Test Content-Addressed DFS Block Storage
    const uint8_t dfs_data[] = "Immutable block data for distributed DAG storage.";
    if (dfs_storage_store_block(0, dfs_data, sizeof(dfs_data), &id_out) != 0) return -105;

    // 6. Test Hardware Accelerator & AI Silicon Registration
    if (accelerator_register_device(1, &id_out) != 0) return -106;
    if (ai_load_balancer_register_device(2, &id_out) != 0) return -107;

    // 7. Test MicroVM Sandbox Enclave Creation
    if (microvm_create_enclave("secure_enclave_01", 2, 4096 * 1024, &id_out) != 0) return -108;

    // 8. Test AuraScript Workflow Loading
    const uint8_t script_code[] = {1, 2, 3, 4}; // OP_ALLOC_VM, OP_DISPATCH_AI, etc.
    if (aurascript_load_script(script_code, sizeof(script_code), &id_out) != 0) return -109;

    // 9. Test ZKP Provenance Submission
    const uint8_t stmt[] = "Compute verified without state leak";
    const uint8_t proof[] = "zkp_proof_bytes_signature_vector";
    if (zkp_provenance_submit(1, stmt, sizeof(stmt), proof, sizeof(proof), &id_out) != 0) return -110;

    // 10. Test Kernel RLE Compression & Archive
    const uint8_t raw_data[] = "AAAAABBBBBCCCCCDDDDD";
    size_t compressed_len = sizeof(scratch);
    if (compression_zip(raw_data, sizeof(raw_data), scratch, &compressed_len) != 0) return -111;

    // 11. Test Kernel-Bypass Zero-Copy Networking Packet Transmission
    if (kernel_bypass_transmit(0x02, raw_data, 16, &id_out) != 0) return -112;

    // 12. Test Asynchronous Event Reactor Registration
    if (event_reactor_register(900, 1, 10, &id_out) != 0) return -113;

    return 0; // All TorusOS subsystem self-tests passed successfully
}
