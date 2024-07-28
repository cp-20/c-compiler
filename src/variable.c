#include "variable.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "error.h"
#include "parser.h"
#include "vector.h"

Variable* new_variable(int reg, Type type, Variable* ptr_to, int array_size) {
  Variable* var = calloc(1, sizeof(Variable));
  var->name = NULL;
  var->len = 0;
  var->reg = reg;
  var->type = type;
  var->ptr_to = ptr_to;
  var->array_size = array_size;
  var->value = NULL;
  return var;
}

Variable* with_reg(Variable* var, int reg) {
  Variable* new_var = copy_var(var);
  new_var->reg = reg;
  return new_var;
}

Variable* copy_var_if_needed(Variable* var) {
  if (var == NULL) return NULL;
  if (var->type == TYPE_STRUCT) return var;
  return copy_var(var);
}

Variable* copy_var(Variable* var) {
  if (var == NULL) return NULL;
  Variable* new_var = calloc(1, sizeof(Variable));
  new_var->reg = var->reg;
  new_var->type = var->type;
  new_var->array_size = var->array_size;
  new_var->len = var->len;
  new_var->ptr_to = copy_var_if_needed(var->ptr_to);
  if (var->name != NULL) {
    new_var->name = calloc(var->len + 1, sizeof(char));
    sprintf(new_var->name, "%.*s", var->len, var->name);
  }
  if (var->value != NULL) {
    new_var->value = calloc(1, sizeof(int));
    *new_var->value = *var->value;
  }
  if (var->fields != NULL) {
    new_var->fields = new_vector();
    for (int i = 0; i < var->fields->size; i++) {
      LVar* field = vec_at(var->fields, i);
      LVar* new_field = calloc(1, sizeof(LVar));
      new_field->name = calloc(field->len + 1, sizeof(char));
      sprintf(new_field->name, "%.*s", field->len, field->name);
      new_field->len = field->len;
      new_field->offset = field->offset;
      new_field->var = copy_var_if_needed(field->var);
      vec_push_last(new_var->fields, new_field);
    }
  }
  return new_var;
}

void push_variable(vector* stack, Variable* var) { vec_push_last(stack, var); }

Variable* pop_variable(vector* stack) { return vec_pop(stack); }

Variable* get_last_variable(vector* stack) { return vec_last(stack); }

char* get_variable_type_str(Variable* var) {
  switch (var->type) {
    case TYPE_VOID: {
      char* type = calloc(5, sizeof(char));
      sprintf(type, "void");
      return type;
    }
    case TYPE_I8: {
      char* type = calloc(3, sizeof(char));
      sprintf(type, "i8");
      return type;
    }
    case TYPE_I32: {
      char* type = calloc(4, sizeof(char));
      sprintf(type, "i32");
      return type;
    }
    case TYPE_PTR: {
      Variable* p = var;
      while (p != NULL) {
        p = p->ptr_to;
        if (p->type == TYPE_VOID) {
          char* type = calloc(4, sizeof(char));
          sprintf(type, "ptr");
          return type;
        }
        if (p->type != TYPE_PTR) break;
      }
      char* ptr_to_type = get_variable_type_str(var->ptr_to);
      char* type = calloc(strlen(ptr_to_type) + 2, sizeof(char));
      sprintf(type, "%s*", ptr_to_type);
      free(ptr_to_type);
      return type;
    }
    case TYPE_ARRAY: {
      char* type = calloc(256, sizeof(char));
      char* ptr_to_type = get_variable_type_str(var->ptr_to);
      sprintf(type, "[%d x %s]", var->array_size, ptr_to_type);
      free(ptr_to_type);
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
  Variable* ptr_var = new_variable(
      -1, TYPE_PTR, var->type == TYPE_STRUCT ? var : copy_var(var), -1);
  char* result = get_variable_type_str(ptr_var);
  free_variable(ptr_var);
  return result;
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
    if (var1->ptr_to->type == TYPE_VOID || var2->ptr_to->type == TYPE_VOID) {
      return true;
    }
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
      error(
          "演算子の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s, "
          "演算子: %s",
          get_variable_type_str(lval), get_variable_type_str(rval),
          get_node_kind_name(kind));
    }
    return copy_var(lval);
  }

  if (kind == ND_EQ || kind == ND_NE || kind == ND_LT || kind == ND_LE ||
      kind == ND_GT || kind == ND_GE) {
    return new_variable(-1, TYPE_I32, NULL, -1);
  }

  if (lval->type == TYPE_PTR && is_number(rval)) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return copy_var(lval);
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (is_number(lval) && rval->type == TYPE_PTR) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return copy_var(rval);
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (lval->type == TYPE_PTR && rval->type == TYPE_PTR) {
    if (kind == ND_SUB) {
      return new_variable(-1, TYPE_I32, NULL, -1);
    }
    error("ポインタ型の変数に対する演算子が不正です");
  }

  if (lval->type == TYPE_ARRAY && is_number(rval)) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return copy_var(lval);
    }
    error("配列型の変数に対する演算子が不正です");
  }
  if (is_number(lval) && rval->type == TYPE_ARRAY) {
    if (kind == ND_ADD || kind == ND_SUB) {
      return copy_var(rval);
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

LVar* copy_lvar(LVar* lvar) {
  if (lvar == NULL) return NULL;
  LVar* new_lvar = calloc(1, sizeof(LVar));
  new_lvar->name = calloc(lvar->len + 1, sizeof(char));
  sprintf(new_lvar->name, "%.*s", lvar->len, lvar->name);
  new_lvar->len = lvar->len;
  new_lvar->offset = lvar->offset;
  new_lvar->var = copy_var(lvar->var);
  return new_lvar;
}

void free_lvar(LVar* lvar) {
  if (lvar == NULL) return;
  free_variable(lvar->var);
  free(lvar->name);
  free(lvar);
  return;
}

void free_variable(Variable* var) {
  if (var == NULL) return;
  if (var->type == TYPE_STRUCT) return;
  free_variable(var->ptr_to);
  free(var->value);
  // print_debug("free_variable: %p", var->name);
  free(var->name);
  // print_debug("free_variable end: %p", var->name);
  free(var);
}

void free_struct(Variable* var) {
  if (var == NULL) return;
  if (var->fields != NULL) {
    for (int i = 0; i < var->fields->size; i++) {
      free_lvar(vec_at(var->fields, i));
    }
    vec_free(var->fields);
  }
  free(var->name);
  free(var);
}
