#include "lib/vector.h"
#include "llvm/llvm.h"
#include "llvm/variable.h"

#define NULL 0

Variable* new_register_variable(VariableType* type) {
  int number = register_register();
  char* name = malloc(1 + 10);
  sprintf(name, "\%%d", number);

  Variable* variable = malloc(sizeof(Variable));
  variable->type = type;
  variable->name = name;

  return variable;
}

Variable* new_string_variable() {
  int number = register_string();
  char* name = malloc(5 + 10);
  sprintf(name, "@.str%d", number);

  Variable* variable = malloc(sizeof(Variable));
  variable->type =
      create_pointer_variable_type(create_primitive_variable_type(LLVM_VOID));
  variable->name = name;

  return variable;
}

Variable* new_external_variable(char* name) {
  Variable* variable = malloc(sizeof(Variable));
  variable->type =
      create_pointer_variable_type(create_primitive_variable_type(LLVM_VOID));
  variable->name = name;

  return variable;
}

Variable* syntax_init_variable(vector* output, VariableType* type) {
  Variable* variable = new_register_variable(type);
  int size = variable_size(type);

  char* type_str = variable_type(create_pointer_variable_type(type));
  char* format = "  %s = alloca %s, align %s\n";

  char* buffer = malloc(strlen(format) + strlen(variable->name) +
                        strlen(type_str) + strlen(size));
  sprintf(buffer, format, variable->name, type_str, size);

  pushLast(output, buffer);

  return variable;
}

void syntax_assign_value(vector* output, Variable* var, char* val) {
  if (var->type->type != LLVM_POINTER) {
    // TODO: error
    return;
  }
  if (var->type->pointer->type == LLVM_POINTER ||
      var->type->pointer->type == LLVM_VOID) {
    // TODO: error
    return;
  }

  char* format = "  store %s %s, ptr %s, align %s\n";
  char* type_str = variable_type(var->type->pointer->type);
  char* size = variable_size(var->type->pointer->type);

  char* buffer = malloc(strlen(format) + strlen(type_str) + strlen(val) +
                        strlen(var) + strlen(size));
  sprintf(buffer, format, type_str, val, type_str, var, size);

  pushLast(output, buffer);
}

void syntax_assign_variable(vector* output, Variable* v1, Variable* v2) {
  if (v1->type->type != LLVM_POINTER) {
    // TODO: error
    return;
  }
  char* format = "  store %s %s, ptr %s, align %s\n";
  char* type_str = variable_type(v1->type);
  char* size = variable_size(v1->type);

  char* buffer = malloc(strlen(format) + strlen(type_str) + strlen(v2) +
                        strlen(v1) + strlen(size));
  sprintf(buffer, format, type_str, v2, type_str, v1, size);

  pushLast(output, buffer);
}

Variable* syntax_load_variable(vector* output, Variable* src) {
  VariableType* type = get_register_type(src);
  Variable* dest = new_register_variable(type);

  char* format = "  %s = load %s, ptr %s, align %s\n";
  char* type_str = variable_type(type);
  char* size = variable_size(type);

  char* buffer = malloc(strlen(format) + strlen(src->name) + strlen(type_str) +
                        strlen(src) + strlen(size));
  sprintf(buffer, format, dest->name, type_str, src->name, size);

  pushLast(output, buffer);

  return src;
}

void syntax_compare(vector* output, Variable* v1, Variable* v2, char* cond) {
  if (v1->type->type != v2->type->type) {
    // TODO: error
    return;
  }
  char* format = "  %s = icmp %s %s %s, %s\n";
  char* type_str = variable_type(v1->type->type);

  char* buffer = malloc(strlen(format) + strlen(type_str) + strlen(v1->name) +
                        strlen(v2->name) + strlen(cond));
  sprintf(buffer, format, cond, type_str, v1->name, v2->name);

  pushLast(output, buffer);
}

Variable* syntax_init_string(vector* output, char* value) {
  Variable* variable = new_string_variable();

  int length_number = strlen(value);
  char* length = malloc(10);
  sprintf(length, "%d", length_number);

  char* format =
      "  %s = private unnamed_addr constant [%s x i8] c\"%s\\00\", align 1\n";
  char* buffer = malloc(strlen(format) + strlen(variable) + strlen(length) +
                        strlen(value));
  sprintf(buffer, format, variable, strlen(value), value);

  pushLast(output, buffer);

  return variable;
}

char* syntax_label(vector* output) {
  int number = register_register();
  char* name = malloc(1 + 10);
  sprintf(name, "\%%d", number);

  char* format = "%s:\n";
  char* buffer = malloc(strlen(format) + strlen(name) - 1);
  sprintf(buffer, format, name + 1);

  pushLast(output, buffer);

  return name;
}

void syntax_if_start(vector* output, Variable* cond, char* true_label,
                     char* false_label) {
  char* format = "  br i1 %s, label \%%s, label \%%s\n";
  char* buffer = malloc(strlen(format) + strlen(cond->name) +
                        strlen(true_label) + strlen(false_label));
  sprintf(buffer, format, cond->name, true_label, false_label);

  pushLast(output, buffer);
}

void syntax_jump(vector* output, char* label) {
  char* format = "  br label \%%s\n";
  char* buffer = malloc(strlen(format) + strlen(label));
  sprintf(buffer, format, label);

  pushLast(output, buffer);
}

void syntax_function_start(vector* output, char* name,
                           VariableType* return_type, Variable** args) {
  char* format = "define dso_local %s @%s(%s) #0 {\n";
  char* args_str = "";
  for (int i = 0; args[i] != NULL; i++) {
    char* type = variable_type(args[i]->type);
    char* name = args[i]->name;
    char* buffer = malloc(strlen(type) + strlen(name) + 3);
    sprintf(buffer, "%s %s", type, name);
    if (i != 0) {
      args_str = realloc(args_str, strlen(args_str) + 2);
      strcat(args_str, ", ");
    }
    args_str = realloc(args_str, strlen(args_str) + strlen(buffer) + 1);
    strcat(args_str, buffer);
  }
}

void syntax_function_end(vector* output) { pushLast(output, "}\n"); }