#ifndef OMNI_ENVELOPE_H
#define OMNI_ENVELOPE_H

#include <stdint.h>
#include <stdbool.h>

#define OMNI_MAX_TAG_LEN 32
#define OMNI_MAX_CONTENT_LEN 256

typedef struct {
    char tag[OMNI_MAX_TAG_LEN];
    char content[OMNI_MAX_CONTENT_LEN];
    uint64_t checksum;
} OmniEnvelope;

void omni_envelope_init(void);
bool omni_parse_ascii_envelope(const char *raw_xml, OmniEnvelope *out_env);
uint64_t omni_compute_envelope_checksum(const OmniEnvelope *env);

#endif
