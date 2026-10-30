#ifndef TRUST_CORE_PQC_H
#define TRUST_CORE_PQC_H

#include <stdint.h>
#include <stddef.h>

#define ML_DSA_65_PUBLIC_KEY_BYTES  1952
#define ML_DSA_65_SIGNATURE_BYTES   3293
#define PQC_SCRATCH_ARENA_SIZE      16384 /* 16KB Safe Kernel Limit */

#define TRUST_CORE_SUCCESS              0
#define TRUST_CORE_ERR_NULL_POINTER    -1
#define TRUST_CORE_ERR_SIG_LENGTH      -2
#define TRUST_CORE_ERR_RESOURCE        -3
#define TRUST_CORE_ERR_CRYPTO_FAIL     -4

typedef struct {
    uint8_t scratch_pad[PQC_SCRATCH_ARENA_SIZE];
    size_t  offset;
} pqc_workspace_t;

int trust_core_verify_pqc_signature(
    const uint8_t *payload, 
    size_t payload_len, 
    const uint8_t *signature, 
    size_t sig_len,
    const uint8_t *public_key
);

#endif /* TRUST_CORE_PQC_H */
