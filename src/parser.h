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
  ND_AND,     // &&
  ND_OR,      // ||
  ND_ASSIGN,  // = (代入)
  ND_LVAR,    // ローカル変数
  ND_RETURN,  // return
  ND_IF,      // if
  ND_WHILE,   // while
  ND_FOR,     // for
  ND_BLOCK,   // { ... }
  ND_CALL,    // 関数呼び出し
  ND_NUM,     // 整数
} NodeKind;

// 関数定義用のノード
typedef struct Function Function;
struct Function {
  char *name;      // 関数名
  int len;         // 名前の長さ
  int argc;        // 引数の数 (localsの最初のargc個が引数)
  vector *locals;  // ローカル変数のリスト
  vector *body;    // 関数の本体
};

// ND_CALLのときの型
typedef struct Call Call;
struct Call {
  char *name;    // 関数名
  int len;       // 名前の長さ
  vector *args;  // 引数のリスト
};

// 抽象構文木のノードの型
typedef struct Node Node;
struct Node {
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  Node *extra;    // 追加のノード
  Node *extra2;   // 追加のノード
  Call *call;     // kindがND_CALLのときのみ使う
  vector *stmts;  // 後に続くstatementのリスト
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがND_LVARの場合のみ使う
  NodeKind kind;  // ノードの型
};

// ローカル変数の型
typedef struct LVar LVar;
struct LVar {
  char *name;  // 変数の名前
  int len;     // 名前の長さ
  int offset;  // 何番目に宣言された変数か (0-indexed)
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
