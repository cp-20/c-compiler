#include "variable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
