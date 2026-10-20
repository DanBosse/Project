#ifndef ASCII_XML_H
#define ASCII_XML_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    char key[32];
    char val[32];
} AsciiXmlAttr;

typedef struct {
    char tag[32];
    AsciiXmlAttr attrs[4]; // Support up to 4 attributes per element
    int attr_count;
    char value[128];
} AsciiXmlElement;

int ascii_validate_7bit_strict(const char *data, size_t length, size_t *out_fault_offset);
int ascii_xml_parse_node_safe(const char *xml_str, AsciiXmlElement *out_element, size_t *out_error_offset);

#endif
