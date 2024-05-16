#pragma once

#include <stdio.h>

#include "llvm.h"
#include "parser.h"
#include "vector.h"

int generate(vector* code);

int generate_node(Node* node, vector* stack, int* locals);