#include "variable.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "vector.h"

void push_variable(vector* stack, int reg, Type type, int ref_nest) {
  Variable* var = calloc(1, sizeof(Variable));
  var->reg = reg;
  var->type = type;
  var->ref_nest = ref_nest;
  vec_push_last(stack, var);
}

Variable* pop_variable(vector* stack) { return vec_pop(stack); }

Variable* get_last_variable(vector* stack) { return vec_last(stack); }

char* get_variable_type_str(Variable* var, int ref_nest_diff) {
  int ref_nest = var->ref_nest + ref_nest_diff;

  char base[4];
  switch (var->type) {
    case TYPE_I32:
      sprintf(base, "i32");
  }

  char pointer[ref_nest + 1];
  for (int i = 0; i < ref_nest; i++) {
    pointer[i] = '*';
  }
  pointer[ref_nest] = '\0';

  char* type = calloc(strlen(base) + ref_nest + 1, sizeof(char));
  sprintf(type, "%s%s", base, pointer);

  return type;
}

int get_variable_size(Variable* var, int ref_nest_diff) {
  int ref_nest = var->ref_nest + ref_nest_diff;
  if (ref_nest > 0) {
    return 8;
  }

  switch (var->type) {
    case TYPE_I32:
      return 4;
  }
  return 0;
}

bool is_same_type(Variable* var1, Variable* var2) {
  return var1->type == var2->type && var1->ref_nest == var2->ref_nest;
}

Variable* get_calc_result_type(NodeKind kind, Variable* lval, Variable* rval) {
  if (lval->ref_nest == 0 && rval->ref_nest == 0) {
    if (lval->type != rval->type) {
      error("演算子の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s",
            get_variable_type_str(lval, 0), get_variable_type_str(rval, 0));
    }
    return lval;
  }

  if (lval->ref_nest > 0 && rval->ref_nest == 0) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return lval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (lval->ref_nest == 0 && rval->ref_nest > 0) {
    if (kind == ND_ADD) {
      return rval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (lval->ref_nest > 0 && rval->ref_nest > 0) {
    if (kind == ND_SUB) {
      return lval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  return NULL;
}
