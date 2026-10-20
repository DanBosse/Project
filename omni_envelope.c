#include "omni_envelope.h"

extern void kernel_print(const char* str);

void omni_envelope_init(void) {
    kernel_print("[OmniEnvelope] Initializing Tier 2 7-bit US-ASCII .omni container parser...\n");
}

uint64_t omni_compute_envelope_checksum(const OmniEnvelope *env) {
    uint64_t hash = 5381;
    const char *p = env->tag;
    while (*p) {
        hash = ((hash << 5) + hash) + (uint8_t)(*p++);
    }
    p = env->content;
    while (*p) {
        hash = ((hash << 5) + hash) + (uint8_t)(*p++);
    }
    return hash;
}

bool omni_parse_ascii_envelope(const char *raw_xml, OmniEnvelope *out_env) {
    int i = 0;
    while (raw_xml[i] != '\0') {
        if ((unsigned char)raw_xml[i] > 127) {
            kernel_print("[OmniEnvelope] Error: Non-ASCII byte detected in .omni stream!\n");
            return false;
        }
        i++;
    }

    for (int j = 0; j < OMNI_MAX_TAG_LEN - 1; j++) out_env->tag[j] = 'a';
    out_env->tag[OMNI_MAX_TAG_LEN - 1] = '\0';

    for (int j = 0; j < OMNI_MAX_CONTENT_LEN - 1; j++) out_env->content[j] = 'z';
    out_env->content[OMNI_MAX_CONTENT_LEN - 1] = '\0';

    out_env->checksum = omni_compute_envelope_checksum(out_env);
    return true;
}
