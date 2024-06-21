#include "variable.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "vector.h"

Variable* new_variable(int reg, Type type, Variable* ptr_to, int array_size) {
  Variable* var = calloc(1, sizeof(Variable));
  var->reg = reg;
  var->type = type;
  var->ptr_to = ptr_to;
  var->array_size = array_size;
  return var;
}

Variable* with_reg(Variable* var, int reg) {
  Variable* new_var =
      new_variable(reg, var->type, var->ptr_to, var->array_size);
  return new_var;
}

void push_variable(vector* stack, Variable* var) { vec_push_last(stack, var); }

Variable* pop_variable(vector* stack) { return vec_pop(stack); }

Variable* get_last_variable(vector* stack) { return vec_last(stack); }

char* get_variable_type_str(Variable* var) {
  switch (var->type) {
    case TYPE_I32: {
      return "i32";
    }
    case TYPE_PTR: {
      char* type = calloc(1, 256);
      sprintf(type, "%s*", get_variable_type_str(var->ptr_to));
      return type;
    }
    case TYPE_ARRAY: {
      char* type = calloc(1, 256);
      sprintf(type, "[%d x %s]", var->array_size,
              get_variable_type_str(var->ptr_to));
      return type;
    }
  }
}

int get_variable_size(Variable* var) {
  if (var->type == TYPE_ARRAY) {
    return get_variable_size(var->ptr_to) * var->array_size;
  }

  if (var->type == TYPE_PTR) {
    return 8;
  }

  switch (var->type) {
    case TYPE_I32:
      return 4;
    default:
      return 0;
  }
}

bool is_same_type(Variable* var1, Variable* var2) {
  if (var1->type != var2->type) {
    return false;
  }

  if (var1->type == TYPE_PTR) {
    return is_same_type(var1->ptr_to, var2->ptr_to);
  }

  if (var1->type == TYPE_ARRAY) {
    return var1->array_size == var2->array_size &&
           is_same_type(var1->ptr_to, var2->ptr_to);
  }

  return true;
}

Variable* get_calc_result_type(NodeKind kind, Variable* lval, Variable* rval) {
  if (lval->type != TYPE_PTR && rval->type != TYPE_PTR) {
    if (!is_same_type(lval, rval)) {
      error("演算子の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s",
            get_variable_type_str(lval), get_variable_type_str(rval));
    }
    return lval;
  }

  if (lval->type == TYPE_PTR && rval->type != TYPE_PTR) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return lval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (lval->type != TYPE_PTR && rval->type == TYPE_PTR) {
    if (kind == ND_ADD) {
      return rval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (lval->type == TYPE_PTR && rval->type == TYPE_PTR) {
    if (kind == ND_SUB) {
      return lval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  return NULL;
}
