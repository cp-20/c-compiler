#pragma once

#include <stdbool.h>

#include "code.h"
#include "node-kind.h"
#include "variable.h"
#include "vector.h"

// 関数定義用のノード
typedef struct Function Function;
struct Function {
  char *name;        // 関数名
  int len;           // 名前の長さ
  int argc;          // 引数の数 (localsの最初のargc個が引数)
  bool have_va_arg;  // 可変長引数を持つか
  bool is_proto;     // プロトタイプ宣言かどうか
  Variable *ret;     // 戻り値の型
  vector *locals;    // ローカル変数のリスト
  vector *structs;   // 構造体のリスト
  vector *body;      // 関数の本体
};

// ND_CALLのときの型
typedef struct Call Call;
struct Call {
  char *name;     // 関数名
  int len;        // 名前の長さ
  Variable *ret;  // 戻り値の型
  vector *args;   // 引数のリスト
};

// 抽象構文木のノードの型
typedef struct Node Node;
struct Node {
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  Node *extra;    // 追加のノード
  Node *extra2;   // 追加のノード
  Call *call;     // kindがND_CALLのときのみ使う
  vector *stmts;  // 後に続くstatementのリスト (switchならcase/default)
  vector *locals;  // ローカル変数のリスト (スコープを作る時だけ)
  Variable *cast;  // キャストする型 (キャストの時のみ)
  int val;         // kindがND_NUMの場合のみ使う
  int offset;      // kindがND_LVARの場合のみ使う
  NodeKind kind;   // ノードの型
};

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);

Node *new_node_num(int val);

Code *print_node(Node *node);

char *get_node_kind_name(NodeKind kind);

void free_call(Call *call);

void free_node(Node *node);

void free_function(Function *func);

Call *copy_call(Call *call);

Node *copy_node(Node *node);
