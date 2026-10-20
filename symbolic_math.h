#ifndef SYMBOLIC_MATH_H
#define SYMBOLIC_MATH_H

#include <stdint.h>

typedef enum {
    SYM_TYPE_CONSTANT,
    SYM_TYPE_VARIABLE,
    SYM_TYPE_ADD,
    SYM_TYPE_MULTIPLY,
    SYM_TYPE_MANIFOLD_PSI
} SymbolType;

typedef struct SymbolicNode {
    SymbolType type;
    int64_t value;          // For constants
    char name;              // For variables (e.g., 'x', 't')
    struct SymbolicNode *left;
    struct SymbolicNode *right;
} SymbolicNode;

void symbolic_math_init(void);
SymbolicNode* symbolic_create_constant(int64_t val);
SymbolicNode* symbolic_create_variable(char name);
SymbolicNode* symbolic_create_binary(SymbolType type, SymbolicNode *left, SymbolicNode *right);
int64_t symbolic_evaluate(SymbolicNode *node, int64_t var_x_val);

#endif
