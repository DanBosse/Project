#ifndef AURASCRIPT_CLAMP_H
#define AURASCRIPT_CLAMP_H

void aurascript_clamp_init(void);
int aurascript_register_clamp_rule(const char* property, const char* min_val, const char* preferred_val, const char* max_val);

#endif
