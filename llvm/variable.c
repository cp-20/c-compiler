#include "llvm.h"

typedef enum PrimitiveVariableType {
  LLVM_INT,
  LLVM_VOID,
  LLVM_POINTER
} PrimitiveVariableType;

typedef struct VariableType {
  PrimitiveVariableType type;
  VariableType* pointer;
} VariableType;

VariableType* create_primitive_variable_type(PrimitiveVariableType type) {
  VariableType* variable = malloc(sizeof(VariableType));
  variable->type = type;
  return variable;
}

VariableType* create_pointer_variable_type(VariableType* type) {
  VariableType* pointer = malloc(sizeof(VariableType));
  pointer->type = LLVM_POINTER;
  pointer->pointer = type;
  return pointer;
}

typedef struct Variable {
  VariableType* type;
  char* name;
} Variable;

char* variable_type(VariableType* type) {
  switch (type->type) {
    case LLVM_INT:
      return "i32";
    case LLVM_POINTER:
      return "ptr";
  }
}

char variable_load_type(VariableType* type) {
  switch (type->type) {
    case LLVM_INT:
      return "i32";
    case LLVM_VOID:
      return "ptr";
    case LLVM_POINTER:
      return variable_load_type(type->pointer);
  }
}

char* variable_size(VariableType* type) {
  switch (type->type) {
    case LLVM_INT:
      return "4";
    case LLVM_POINTER:
      return variable_size(type->pointer);
  }
}
