#ifndef OMNI_ENVELOPE_H
#define OMNI_ENVELOPE_H

#include <stdint.h>
#include <stddef.h>

#define OMNI_ENVELOPE_MAGIC 0x544F52553A454E56ULL /* "TORU:ENV" ASCII representation */
#define OMNI_SUCCESS        0
#define OMNI_ERR_MALFORMED_PACKET -10
#define OMNI_ERR_INVALID_HEADER   -11
#define OMNI_ERR_SIZE_MISMATCH    -12
#define OMNI_ERR_UNTRUSTED_SOURCE -13
#define OMNI_ERR_CRYPTO_REJECTED  -14

typedef struct {
    uint64_t magic;
    uint32_t version;
    uint32_t source_subsystem_id;
    uint32_t target_subsystem_id;
    size_t   payload_len;
} omni_envelope_header_t;

typedef struct {
    omni_envelope_header_t header;
    const uint8_t *payload;
    uint8_t is_verified;
} omni_envelope_t;

int omni_envelope_receive_and_verify(
    const uint8_t *raw_packet, 
    size_t packet_len, 
    omni_envelope_t *out_env
);

#endif /* OMNI_ENVELOPE_H */
