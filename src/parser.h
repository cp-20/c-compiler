#pragma once

#include "node.h"
#include "tokenizer.h"
#include "variable.h"

// ローカル変数の型
typedef struct LVar LVar;
struct LVar {
  char *name;     // 変数の名前
  int len;        // 名前の長さ
  int offset;     // 何番目に宣言された変数か (0-indexed)
  Variable *var;  // 変数の情報
};

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok, vector *locals);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

vector *program(Token **token);
Function *function(Token **token);
Node *stmt(Token **token);
Node *expr(Token **token);
Node *logical(Token **token);
Node *assign(Token **token);
Node *equality(Token **token);
Node *relational(Token **token);
Node *add(Token **token);
Node *mul(Token **token);
Node *unary(Token **token);
Node *primary(Token **token);

vector *parse(Token *token);

void print_node(Node *node);
