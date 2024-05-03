#pragma once

#include <stdio.h>

#include "llvm.h"
#include "parser.h"
#include "vector.h"

int generate(Node* node, vector* stack);