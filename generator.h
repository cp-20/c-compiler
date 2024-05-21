#pragma once

#include <stdio.h>

#include "llvm.h"
#include "parser.h"
#include "vector.h"

int gen_lval(Node* node, int* locals_r);

extern bool returned;

char* generate_node(Node* node, vector* stack, int* locals);

void generate_header(void);

void generate_printf(void);

void generate(vector* code);
