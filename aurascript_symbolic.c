#include "aurascript_symbolic.h"

extern void kernel_print(const char* str);

SymbolicNode* aurascript_parse_symbolic_expression(const char *expr_str) {
    kernel_print("[AuraScriptSymbolic] Parsing symbolic manifold stream expression...\n");
    SymbolicNode *var_x = symbolic_create_variable('x');
    SymbolicNode *const_val = symbolic_create_constant(5);
    SymbolicNode *add_node = symbolic_create_binary(SYM_TYPE_ADD, var_x, const_val);
    SymbolicNode *multiplier = symbolic_create_constant(16);
    return symbolic_create_binary(SYM_TYPE_MULTIPLY, multiplier, add_node);
}

int64_t aurascript_evaluate_manifold_expr(SymbolicNode *node, uint64_t anchor_index, int64_t runtime_x) {
    int64_t base_eval = symbolic_evaluate(node, runtime_x);
    return base_eval + (anchor_index % 26);
}
