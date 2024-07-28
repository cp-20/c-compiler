#pragma once

#include <stdio.h>

#include "code.h"
#include "llvm.h"
#include "parser.h"
#include "vector.h"

Variable* gen_lval(Code* code, Node* node, vector* stack, Variable** locals_r,
                   int* rctx);

void push_variable_with_cast_if_needed(vector* stack, Variable* var,
                                       Variable* cast);

Code* generate_node(Node* node, vector* stack, Variable** locals_r, int* rctx);

Code* generate_struct(Variable* var);
Code* generate_global(LVar* var);
Code* generate_string(Token* tok, int index);
Code* generate_func(Function* func);
Code* generate_header();
Code* generate_lib_functions();

Code* generate(Program* program);
