#include "variable.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
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
  new_var->name = var->name;
  new_var->len = var->len;
  new_var->fields = var->fields;
  return new_var;
}

void push_variable(vector* stack, Variable* var) { vec_push_last(stack, var); }

Variable* pop_variable(vector* stack) { return vec_pop(stack); }

Variable* get_last_variable(vector* stack) { return vec_last(stack); }

char* get_variable_type_str(Variable* var) {
  switch (var->type) {
    case TYPE_VOID:
      return "void";
    case TYPE_I8: {
      return "i8";
    }
    case TYPE_I32: {
      return "i32";
    }
    case TYPE_PTR: {
      Variable* p = var;
      while (p != NULL) {
        p = p->ptr_to;
        if (p->type == TYPE_VOID) return "ptr";
        if (p->type != TYPE_PTR) break;
      }
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
    case TYPE_STRUCT: {
      char* type = calloc(1, var->len + 9);
      sprintf(type, "%%struct.%.*s", var->len, var->name);
      return type;
    }
  }
}

char* get_ptr_variable_type_str(Variable* var) {
  return get_variable_type_str(new_variable(-1, TYPE_PTR, var, -1));
}

int get_variable_size(Variable* var) {
  if (var->type == TYPE_ARRAY) {
    return align(get_variable_size(var->ptr_to) * var->array_size);
  }

  if (var->type == TYPE_STRUCT) {
    int size = 0;
    for (int i = 0; i < var->fields->size; i++) {
      LVar* field = vec_at(var->fields, i);
      size += get_variable_size(field->var);
    }
    return align(size);
  }

  if (var->type == TYPE_PTR) {
    return 8;
  }

  switch (var->type) {
    case TYPE_I8:
      return 1;
    case TYPE_I32:
      return 4;
    case TYPE_VOID:
      error("void型の変数は使えません");
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

bool is_pointer_like(Variable* var) {
  return var->type == TYPE_PTR || var->type == TYPE_ARRAY;
}

bool is_number(Variable* var) {
  return var->type == TYPE_I8 || var->type == TYPE_I32;
}

Variable* get_calc_result_type(NodeKind kind, Variable* lval, Variable* rval) {
  if (!is_pointer_like(lval) && !is_pointer_like(rval)) {
    if (!is_same_type(lval, rval)) {
      error("演算子の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s",
            get_variable_type_str(lval), get_variable_type_str(rval));
    }
    return lval;
  }

  if (kind == ND_EQ || kind == ND_NE || kind == ND_LT || kind == ND_LE ||
      kind == ND_GT || kind == ND_GE) {
    return new_variable(-1, TYPE_I32, NULL, -1);
  }

  if (lval->type == TYPE_PTR && is_number(rval)) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return lval;
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (is_number(lval) && rval->type == TYPE_PTR) {
    if (kind == ND_ADD || kind == ND_SUB) {
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

  if (lval->type == TYPE_ARRAY && is_number(rval)) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return lval;
    }
    error("配列型の変数に対する演算子が不正です");
  }
  if (is_number(lval) && rval->type == TYPE_ARRAY) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return rval;
    }
    error("配列型の変数に対する演算子が不正です");
  }

  return NULL;
}

int pow2[] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512};
int align(int size) {
  for (int i = 0; i < 10; i++) {
    if (size <= pow2[i]) {
      return pow2[i];
    }
  }
  error("アラインメントのサイズが大きすぎます");
  return 0;
}
