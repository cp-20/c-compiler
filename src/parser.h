#pragma once

#include "node.h"
#include "tokenizer.h"
#include "variable.h"

// プログラム
typedef struct Program {
  vector *structs;    // 構造体のリスト
  vector *globals;    // グローバル変数のリスト
  vector *functions;  // 関数のリスト
} Program;

// ローカル変数の型
typedef struct LVar LVar;
struct LVar {
  char *name;     // 変数の名前
  int len;        // 名前の長さ
  int offset;     // 何番目に宣言された変数か (0-indexed)
  Variable *var;  // 変数の情報
};

void print_debug_token(char *type, Token **token);

Variable *find_struct_from_vector(Token *tok, vector *structs);
Variable *find_struct(Token *tok);
int find_struct_field(Token *tok, Variable *struct_var);

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar_from_vector(Token *tok, vector *locals);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

Program *program(Token **token);
Function *global_decl(Token **token);
Node *stmt(Token **token);
Node *expr(Token **token);
Node *local_decl(Token **token);
Node *logical(Token **token);
Node *assign(Token **token);
Node *equality(Token **token);
Node *relational(Token **token);
Node *add(Token **token);
Node *mul(Token **token);
Node *unary(Token **token);
Node *primary(Token **token);
Node *parse_primary_access(Token **token, Node *node);
Variable *parse_struct(Token **token, bool name_required);
Variable *type(Token **token);

Program *parse(Token *token);

void print_node(Node *node);
