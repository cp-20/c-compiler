#pragma once

#include "vector.h"

typedef enum {
  TYPE_I32,
} Type;

typedef struct Variable {
  int reg;       // レジスタ番号
  Type type;     // 変数の型 (参照先の型)
  int ref_nest;  // 参照のネスト数 (1以上ならポインタ)
} Variable;

void push_variable(vector* stack, int reg, Type type, int ref_nest);

Variable* pop_variable(vector* stack);

Variable* get_last_variable(vector* stack);

char* get_variable_type_str(Variable* var, int ref_nest_diff);

int get_variable_size(Variable* var, int ref_nest_diff);
