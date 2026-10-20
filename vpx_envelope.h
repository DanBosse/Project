#ifndef VPX_ENVELOPE_H
#define VPX_ENVELOPE_H

#include <stdint.h>
#include <stdbool.h>

#define VPX_MAX_TAG_LEN 32
#define VPX_MAX_CONTENT_LEN 256

typedef struct {
    char tag[VPX_MAX_TAG_LEN];
    char content[VPX_MAX_CONTENT_LEN];
    uint64_t checksum;
} VpxEnvelope;

void vpx_envelope_init(void);
bool vpx_parse_ascii_envelope(const char *raw_xml, VpxEnvelope *out_env);
uint64_t vpx_compute_envelope_checksum(const VpxEnvelope *env);

#endif
