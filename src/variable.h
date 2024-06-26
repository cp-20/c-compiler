#pragma once

#include <stdbool.h>

#include "node.h"
#include "variable.h"
#include "vector.h"

typedef enum {
  TYPE_I32,
  TYPE_PTR,
  TYPE_ARRAY,
} Type;

typedef struct Variable Variable;

struct Variable {
  int reg;           // レジスタ番号
  Type type;         // 変数の型 (参照先の型)
  Variable* ptr_to;  // 参照先
  int array_size;    // 配列の要素数
};

Variable* new_variable(int reg, Type type, Variable* ptr_to, int array_size);

Variable* with_reg(Variable* var, int reg);

void push_variable(vector* stack, Variable* var);

Variable* pop_variable(vector* stack);

Variable* get_last_variable(vector* stack);

char* get_variable_type_str(Variable* var);

int get_variable_size(Variable* var);

bool is_same_type(Variable* var1, Variable* var2);

bool is_pointer_like(Variable* var);

bool is_number(Variable* var);

Variable* get_calc_result_type(NodeKind kind, Variable* lval, Variable* rval);

int align(int size);
