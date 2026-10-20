#ifndef AURASCRIPT_SYMBOLIC_H
#define AURASCRIPT_SYMBOLIC_H

#include "symbolic_math.h"
#include <stdint.h>

SymbolicNode* aurascript_parse_symbolic_expression(const char *expr_str);
int64_t aurascript_evaluate_manifold_expr(SymbolicNode *node, uint64_t anchor_index, int64_t runtime_x);

#endif
