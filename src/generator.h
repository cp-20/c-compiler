#pragma once

#include <stdio.h>

#include "code.h"
#include "llvm.h"
#include "parser.h"
#include "vector.h"

int gen_lval(Node* node, int* locals_r);

Code* generate_node(Node* node, vector* stack, int* locals_r, rctx rctx);

void generate_func(Function* func);

void generate_header(void);

void generate_print(void);

void generate(vector* code);
