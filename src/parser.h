#pragma once

#include "node.h"
#include "tokenizer.h"
#include "variable.h"

extern vector *global_locals;
extern vector *global_structs;
extern vector *global_typedefs;
extern vector *global_strings;
extern vector *global_local_structs;
extern vector *global_globals;
extern int anon_structs_index;

// プログラム
typedef struct Program {
  vector *structs;    // 構造体のリスト
  vector *globals;    // グローバル変数のリスト
  vector *functions;  // 関数のリスト
  vector *typedefs;   // typedefのリスト
  vector *strings;    // 文字列のリスト
} Program;

void print_debug_token(char *type, Token **token);

Variable *find_struct_from_vector(Token *tok, vector *structs);
Variable *find_struct(Token *tok);
int find_struct_field(Token *tok, Variable *struct_var);

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar_from_vector(Token *tok, vector *locals);

int get_offset(void);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

void setup_program(void);
char *function_conversion(char *name);

Program *program(Token **token);
Function *global_decl(Token **token);
Node *stmt(Token **token);
Node *expr(Token **token);
Node *local_decl(Token **token);
Node *ternary(Token **token);
Node *logical(Token **token);
Node *assign(Token **token);
Node *equality(Token **token);
Node *relational(Token **token);
Node *add(Token **token);
Node *mul(Token **token);
Node *unary(Token **token);
Node *primary(Token **token);
Node *parse_primary_access(Token **token, Node *node);
Variable *parse_struct(Token **token, bool name_required, bool is_typedef);
bool enum_decl(Token **token);
Variable *type(Token **token, bool exclude_ptr, bool is_typedef);
Node *constant(Token **token);

Program *parse(Token *token);
