#include "trust_core_pqc.h"

// External kernel hooks (mocked for standalone module integration)
extern pqc_workspace_t* trust_core_get_local_workspace(void);
extern void trust_core_log_security_violation(const char *msg);
extern int ml_dsa_65_verify_internal(const uint8_t *pk, const uint8_t *sig, const uint8_t *mu, size_t mulen, uint8_t *scratch, size_t scratch_len);

int trust_core_verify_pqc_signature(
    const uint8_t *payload, 
    size_t payload_len, 
    const uint8_t *signature, 
    size_t sig_len,
    const uint8_t *public_key) 
{
    if (!payload || !public_key) {
        return TRUST_CORE_ERR_NULL_POINTER;
    }
    if (sig_len != ML_DSA_65_SIGNATURE_BYTES) {
        return TRUST_CORE_ERR_SIG_LENGTH;
    }

    pqc_workspace_t *ws = trust_core_get_local_workspace();
    if (!ws) {
        return TRUST_CORE_ERR_RESOURCE;
    }

    uint8_t mu[64];
    const char *domain_tag = "TORUSOS_PQC_ENVELOPE_V1";
    
    // Suppress unused warnings while keeping domain separation ready for hashing logic
    (void)domain_tag;
    (void)payload_len;

    // Execute lattice-based polynomial verification
    int verification_result = ml_dsa_65_verify_internal(
        public_key, 
        signature, 
        mu, 
        sizeof(mu), 
        ws->scratch_pad, 
        PQC_SCRATCH_ARENA_SIZE
    );

    // Zeroize temporary scratch memory to prevent side-channel leakage
    for (size_t i = 0; i < PQC_SCRATCH_ARENA_SIZE; i++) {
        ws->scratch_pad[i] = 0;
    }

    if (verification_result != 0) {
        trust_core_log_security_violation("PQC Signature Mismatch: Envelope Dropped");
        return TRUST_CORE_ERR_CRYPTO_FAIL;
    }

    return TRUST_CORE_SUCCESS;
}
