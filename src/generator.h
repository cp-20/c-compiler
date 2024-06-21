#pragma once

#include <stdio.h>

#include "code.h"
#include "llvm.h"
#include "parser.h"
#include "vector.h"

Variable* gen_lval(Code* code, Node* node, vector* stack, Variable** locals_r,
                   rctx rctx);

Code* generate_node(Node* node, vector* stack, Variable** locals_r, rctx rctx);

void generate_func(Function* func);

void generate_header(void);

void generate_print(void);

void generate(vector* code);
