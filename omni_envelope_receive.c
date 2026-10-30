#include "omni_envelope.h"
#include "trust_core_pqc.h"

// System security policy registry (stores trusted subsystem public keys)
extern const uint8_t* trust_core_get_authorized_pubkey(uint32_t subsystem_id);
extern void trust_core_log_security_violation(const char *msg);

/**
 * @brief Receives, validates, and unmarshals an inbound omni_envelope.
 * 
 * @param raw_packet Pointer to the incoming raw byte stream from IPC bus or P2P socket.
 * @param packet_len Total length of the raw packet.
 * @param out_env    Pointer to a pre-allocated destination envelope structure if successful.
 * @return int       TRUST_CORE_SUCCESS (0) on success, or a negative error code.
 */
int omni_envelope_receive_and_verify(
    const uint8_t *raw_packet, 
    size_t packet_len, 
    omni_envelope_t *out_env) 
{
    // 1. Basic structural bounds check (ensure header fits)
    if (!raw_packet || packet_len < sizeof(omni_envelope_header_t)) {
        return OMNI_ERR_MALFORMED_PACKET;
    }

    // 2. Cast and parse the header
    const omni_envelope_header_t *header = (const omni_envelope_header_t*)raw_packet;

    // Verify magic bytes and protocol version
    if (header->magic != OMNI_ENVELOPE_MAGIC || header->version != 0x01) {
        trust_core_log_security_violation("Omni-Envelope: Invalid Magic or Version");
        return OMNI_ERR_INVALID_HEADER;
    }

    // 3. Ensure total packet length matches header declarations and signature offsets
    // Layout expected: [Header] [Payload Data] [ML-DSA-65 Signature (3293 bytes)]
    size_t expected_total_len = sizeof(omni_envelope_header_t) + header->payload_len + ML_DSA_65_SIGNATURE_BYTES;
    if (packet_len != expected_total_len) {
        trust_core_log_security_violation("Omni-Envelope: Size Mismatch / Truncated Packet");
        return OMNI_ERR_SIZE_MISMATCH;
    }

    // Define data pointers within the flat raw packet
    const uint8_t *payload_ptr   = raw_packet + sizeof(omni_envelope_header_t);
    const uint8_t *signature_ptr = payload_ptr + header->payload_len;

    // 4. Retrieve the trusted public key corresponding to the source subsystem/node ID
    const uint8_t *pub_key = trust_core_get_authorized_pubkey(header->source_subsystem_id);
    if (!pub_key) {
        trust_core_log_security_violation("Omni-Envelope: Unknown or Untrusted Source ID");
        return OMNI_ERR_UNTRUSTED_SOURCE;
    }

    // 5. Invoke trust_core Post-Quantum Signature Verification
    // This executes the deterministic lattice math using the Ring 0 static scratch arena.
    int pqc_status = trust_core_verify_pqc_signature(
        payload_ptr,
        header->payload_len,
        signature_ptr,
        ML_DSA_65_SIGNATURE_BYTES,
        pub_key
    );

    if (pqc_status != TRUST_CORE_SUCCESS) {
        // trust_core_verify_pqc_signature already logs the cryptographic violation
        return OMNI_ERR_CRYPTO_REJECTED;
    }

    // 6. Cryptographic attestation passed; safely populate the output envelope structure
    out_env->header = *header;
    out_env->payload = payload_ptr;
    out_env->is_verified = 1; // Mark state as cryptographically authentic

    return OMNI_SUCCESS;
}
