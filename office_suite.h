#ifndef OFFICE_SUITE_H
#define OFFICE_SUITE_H

#include <stdint.h>

void office_suite_init(void);
void office_suite_render(void);
void office_suite_type_char(char c);
void office_suite_toggle_comms(void);

void office_suite_inject_telemetry(const char* data);
#endif
