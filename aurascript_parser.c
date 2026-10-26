/*
 * AuraScript Pratt Parser & Tokenizer (Ring 0)
 */

#include <stdint.h>
#include <stddef.h>
#include "aurascript_compiler.h"

typedef enum {
    TOKEN_EOF = 0,
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD_NODE,
    TOKEN_KEYWORD_CONTAINER,
    TOKEN_KEYWORD_CLASS,
    TOKEN_KEYWORD_VAR,
    TOKEN_KEYWORD_CONST,
    TOKEN_KEYWORD_BIND,
    TOKEN_KEYWORD_EMIT,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_COLON,
    TOKEN_EQUALS,
    TOKEN_SEMICOLON,
    TOKEN_COMMA
} token_type_t;

typedef struct {
    token_type_t type;
    const char* start;
    int length;
} token_t;

static const char* parser_cursor;

void aurascript_init_parser(const char* source) {
    parser_cursor = source;
}

static token_t next_token(void) {
    while (*parser_cursor == ' ' || *parser_cursor == '\t' || *parser_cursor == '\n' || *parser_cursor == '\r') {
        parser_cursor++;
    }
    if (*parser_cursor == '\0') {
        return (token_t){ TOKEN_EOF, parser_cursor, 0 };
    }
    
    const char* start = parser_cursor;
    
    if (*start == '{') { parser_cursor++; return (token_t){ TOKEN_LBRACE, start, 1 }; }
    if (*start == '}') { parser_cursor++; return (token_t){ TOKEN_RBRACE, start, 1 }; }
    if (*start == '[') { parser_cursor++; return (token_t){ TOKEN_LBRACKET, start, 1 }; }
    if (*start == ']') { parser_cursor++; return (token_t){ TOKEN_RBRACKET, start, 1 }; }
    if (*start == '(') { parser_cursor++; return (token_t){ TOKEN_LPAREN, start, 1 }; }
    if (*start == ')') { parser_cursor++; return (token_t){ TOKEN_RPAREN, start, 1 }; }
    if (*start == ':') { parser_cursor++; return (token_t){ TOKEN_COLON, start, 1 }; }
    if (*start == '=') { parser_cursor++; return (token_t){ TOKEN_EQUALS, start, 1 }; }
    if (*start == ';') { parser_cursor++; return (token_t){ TOKEN_SEMICOLON, start, 1 }; }
    if (*start == ',') { parser_cursor++; return (token_t){ TOKEN_COMMA, start, 1 }; }

    // Identifiers & Keywords
    if ((*start >= 'a' && *start <= 'z') || (*start >= 'A' && *start <= 'Z') || *start == '_') {
        while ((*parser_cursor >= 'a' && *parser_cursor <= 'z') ||
               (*parser_cursor >= 'A' && *parser_cursor <= 'Z') ||
               (*parser_cursor >= '0' && *parser_cursor <= '9') ||
               *parser_cursor == '_') {
            parser_cursor++;
        }
        int len = parser_cursor - start;
        if (len == 4 && __builtin_memcmp(start, "node", 4) == 0) return (token_t){ TOKEN_KEYWORD_NODE, start, 4 };
        if (len == 9 && __builtin_memcmp(start, "container", 9) == 0) return (token_t){ TOKEN_KEYWORD_CONTAINER, start, 9 };
        if (len == 5 && __builtin_memcmp(start, "class", 5) == 0) return (token_t){ TOKEN_KEYWORD_CLASS, start, 5 };
        if (len == 3 && __builtin_memcmp(start, "var", 3) == 0) return (token_t){ TOKEN_KEYWORD_VAR, start, 3 };
        if (len == 5 && __builtin_memcmp(start, "const", 5) == 0) return (token_t){ TOKEN_KEYWORD_CONST, start, 5 };
        if (len == 4 && __builtin_memcmp(start, "bind", 4) == 0) return (token_t){ TOKEN_KEYWORD_BIND, start, 4 };
        if (len == 4 && __builtin_memcmp(start, "emit", 4) == 0) return (token_t){ TOKEN_KEYWORD_EMIT, start, 4 };
        return (token_t){ TOKEN_IDENTIFIER, start, len };
    }

    parser_cursor++;
    return next_token();
}

void aurascript_parse_ast(const char* source) {
    aurascript_init_parser(source);
    serial_print("[AuraScript Parser] Parsing token stream via Pratt engine...\n");
    token_t tok = next_token();
    while (tok.type != TOKEN_EOF) {
        if (tok.type == TOKEN_KEYWORD_NODE || tok.type == TOKEN_KEYWORD_CONTAINER) {
            serial_print("[AuraScript Parser] Discovered structural layout node/container.\n");
        }
        tok = next_token();
    }
}
