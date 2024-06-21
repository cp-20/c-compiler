#pragma once

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
  ND_INCR,    // ++ (後ろに付く場合)
  ND_DECR,    // -- (後ろに付く場合)
  ND_REF,     // & (アドレス演算子)
  ND_DEREF,   // * (間接参照演算子)
  ND_ASSIGN,  // = (代入)
  ND_LVAR,    // ローカル変数
  ND_RETURN,  // return
  ND_IF,      // if
  ND_WHILE,   // while
  ND_FOR,     // for
  ND_BLOCK,   // { ... }
  ND_GROUP,   // 複数のノードをまとめる
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

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

void print_node(Node *node);
