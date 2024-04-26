typedef enum PrimitiveVariableType {
  LLVM_INT,
  LLVM_VOID,
  LLVM_POINTER
} PrimitiveVariableType;

typedef struct VariableType {
  PrimitiveVariableType type;
  VariableType* pointer;
} VariableType;

VariableType* create_primitive_variable_type(PrimitiveVariableType type);

VariableType* create_pointer_variable_type(VariableType* type);

typedef struct Variable {
  VariableType* type;
  char* name;
} Variable;

char* variable_type(VariableType* type);

char variable_load_type(VariableType* type);

char* variable_size(VariableType* type);
