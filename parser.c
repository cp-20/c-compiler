#include "tokenizer.h"

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,  // +
  ND_SUB,  // -
  ND_MUL,  // *
  ND_DIV,  // /
  ND_NUM,  // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind;  // ノードの型
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  int val;        // kindがND_NUMの場合のみ使う
};

Node *expr(Token **token);
Node *mul(Token **token);
Node *primary(Token **token);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = kind;
  node->lhs = lhs;
  node->rhs = rhs;
  return node;
}

Node *new_node_num(int val) {
  Node *node = calloc(1, sizeof(Node));
  node->kind = ND_NUM;
  node->val = val;
  return node;
}

Node *expr(Token **token) {
  Node *node = mul(token);

  for (;;) {
    if (consume(token, '+'))
      node = new_node(ND_ADD, node, mul(token));
    else if (consume(token, '-'))
      node = new_node(ND_SUB, node, mul(token));
    else
      return node;
  }
}

Node *mul(Token **token) {
  Node *node = primary(token);

  for (;;) {
    if (consume(token, '*'))
      node = new_node(ND_MUL, node, primary(token));
    else if (consume(token, '/'))
      node = new_node(ND_DIV, node, primary(token));
    else
      return node;
  }
}

Node *primary(Token **token) {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume(token, '(')) {
    Node *node = expr(token);
    expect(token, ')');
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number(token));
}

Node *parse(Token *token) { return expr(&token); }

void print_node(Node *node) {
  if (node->kind == ND_NUM) {
    printf("%d", node->val);
  } else {
    printf("(");
    print_node(node->lhs);
    switch (node->kind) {
      case ND_ADD:
        printf("+");
        break;
      case ND_SUB:
        printf("-");
        break;
      case ND_MUL:
        printf("*");
        break;
      case ND_DIV:
        printf("/");
        break;
    }
    print_node(node->rhs);
    printf(")");
  }
}