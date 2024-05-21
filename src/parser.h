#pragma once

#include "tokenizer.h"
#include "vector.h"

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_EQ,      // ==
  ND_NE,      // !=
  ND_LT,      // <
  ND_LE,      // <=
  ND_GT,      // >
  ND_GE,      // >=
  ND_ASSIGN,  // = (代入)
  ND_LVAR,    // ローカル変数
  ND_RETURN,  // return
  ND_IF,      // if
  ND_WHILE,   // while
  ND_FOR,     // for
  ND_BLOCK,   // { ... }
  ND_NUM,     // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  Node *extra;    // 追加のノード
  Node *extra2;   // 追加のノード
  vector *stmts;  // kindがND_BLOCKの場合のみ使う
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがND_LVARの場合のみ使う
  NodeKind kind;  // ノードの型
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

vector *program(Token **token);
Node *stmt(Token **token);
Node *expr(Token **token);
Node *assign(Token **token);
Node *equality(Token **token);
Node *relational(Token **token);
Node *add(Token **token);
Node *mul(Token **token);
Node *unary(Token **token);
Node *primary(Token **token);

vector *parse(Token *token);

void print_node(Node *node);