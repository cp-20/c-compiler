#include "lib/vector.h"
#include "variable.h"

Variable* new_register_variable(VariableType* type);

Variable* new_string_variable();

Variable* new_external_variable(char* name);

Variable* syntax_init_variable(vector* output, VariableType* type);

void syntax_assign_value(vector* output, Variable* var, char* val);

void syntax_assign_variable(vector* output, Variable* v1, Variable* v2);

Variable* syntax_load_variable(vector* output, Variable* v);

void syntax_compare(vector* output, Variable* v1, Variable* v2, char* cond);

int register_string();

char* syntax_init_string(vector* output, char* value);

char* syntax_label(vector* output);

char* syntax_if_start(vector* output, Variable* cond, char* true_label,
                      char* false_label);

void syntax_jump(vector* output, char* label);

void syntax_function_start(vector* output, char* name,
                           VariableType* return_type, Variable** args);

void syntax_function_end(vector* output);