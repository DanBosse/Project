#ifndef AURASCRIPT_TOKENS_H
#define AURASCRIPT_TOKENS_H

void aurascript_tokens_init(void);
int aurascript_register_token(const char* name, const char* value);
const char* aurascript_resolve_token(const char* name);

#endif
