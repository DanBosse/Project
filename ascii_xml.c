#include "ascii_xml.h"

int ascii_validate_7bit_strict(const char *data, size_t length, size_t *out_fault_offset) {
    for (size_t i = 0; i < length; i++) {
        unsigned char c = (unsigned char)data[i];
        if (c > 0x7F || (c < 0x20 && c != '\t' && c != '\n' && c != '\r')) {
            if (out_fault_offset) {
                *out_fault_offset = i;
            }
            return 0; 
        }
    }
    return 1;
}

int ascii_xml_parse_node_safe(const char *xml_str, AsciiXmlElement *out_element, size_t *out_error_offset) {
    if (!xml_str) {
        if (out_error_offset) *out_error_offset = 0;
        return 0;
    }

    // Clear previous fields
    for (int i = 0; i < 32; i++) { 
        out_element->tag[i] = 0; 
    }
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 32; j++) {
            out_element->attrs[i].key[j] = 0;
            out_element->attrs[i].val[j] = 0;
        }
    }
    out_element->attr_count = 0;
    for (int i = 0; i < 128; i++) { 
        out_element->value[i] = 0; 
    }

    size_t len = 0;
    while (xml_str[len] != '\0') { len++; }

    if (!ascii_validate_7bit_strict(xml_str, len, out_error_offset)) {
        return 0;
    }

    const char *p = xml_str;

    // Find opening '<'
    while (*p && *p != '<') p++;
    if (*p != '<') return 0;
    p++; // skip '<'

    // Extract tag name (up to space or '>')
    size_t tag_len = 0;
    while (*p && *p != ' ' && *p != '>' && tag_len < sizeof(out_element->tag) - 1) {
        out_element->tag[tag_len++] = *p++;
    }
    out_element->tag[tag_len] = '\0';

    // Parse attributes loop
    while (*p && *p != '>') {
        while (*p == ' ') p++; // skip spaces
        if (*p == '>' || *p == '\0') break;

        char aname[32];
        size_t an_len = 0;
        while (*p && *p != '=' && *p != ' ' && *p != '>' && an_len < sizeof(aname) - 1) {
            aname[an_len++] = *p++;
        }
        aname[an_len] = '\0';

        while (*p == ' ' || *p == '=') p++; // skip '=' and spaces

        if (*p == '"') {
            p++; // skip opening quote
            char aval[32];
            size_t av_len = 0;
            while (*p && *p != '"' && av_len < sizeof(aval) - 1) {
                aval[av_len++] = *p++;
            }
            aval[av_len] = '\0';
            if (*p == '"') p++; // skip closing quote

            if (out_element->attr_count < 4) {
                int idx = out_element->attr_count;
                for (size_t i = 0; i <= an_len; i++) out_element->attrs[idx].key[i] = aname[i];
                for (size_t i = 0; i <= av_len; i++) out_element->attrs[idx].val[i] = aval[i];
                out_element->attr_count++;
            }
        }
    }

    if (*p == '>') p++; // skip '>'

    // Extract inner value (up to '</')
    size_t val_len = 0;
    while (*p && *p != '<' && val_len < sizeof(out_element->value) - 1) {
        out_element->value[val_len++] = *p++;
    }
    out_element->value[val_len] = '\0';

    return 1;
}
