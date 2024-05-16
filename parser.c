#include "color.h"
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
  ND_NUM,     // 整数
} NodeKind;

typedef struct Node Node;

// 抽象構文木のノードの型
struct Node {
  NodeKind kind;  // ノードの型
  Node *lhs;      // 左辺
  Node *rhs;      // 右辺
  int val;        // kindがND_NUMの場合のみ使う
  int offset;     // kindがND_LVARの場合のみ使う
};

// EBNF grammar expression
//
// program    = stmt*
// stmt       = expr ";" | "return" expr ";"
// expr       = assign
// assign     = equality ("=" assign)?
// equality   = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add        = mul ("+" mul | "-" mul)*
// mul        = unary ("*" unary | "/" unary)*
// unary      = ("+" | "-")? primary
// primary    = num | ident | "(" expr ")"

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

vector *program(Token **token) {
  vector *code = new_vector();
  while ((*token)->kind != TK_EOF) {
    Node *statement = stmt(token);
    vec_push_last(code, statement);
  };
  return code;
}

Node *stmt(Token **token) {
  Node *node;

  if (consume_reserved(token, TK_RETURN)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_RETURN;
    node->lhs = expr(token);
  } else {
    node = expr(token);
  }

  if (!consume(token, ";")) error_at((*token)->str, "';'ではないトークンです");
  return node;
}

Node *expr(Token **token) { return assign(token); }

Node *assign(Token **token) {
  Node *node = equality(token);
  if (consume(token, "=")) node = new_node(ND_ASSIGN, node, assign(token));
  return node;
}

Node *equality(Token **token) {
  Node *node = relational(token);

  for (;;) {
    if (consume(token, "=="))
      node = new_node(ND_EQ, node, relational(token));
    else if (consume(token, "!="))
      node = new_node(ND_NE, node, relational(token));
    else
      return node;
  }
}

Node *relational(Token **token) {
  Node *node = add(token);

  for (;;) {
    if (consume(token, "<"))
      node = new_node(ND_LT, node, add(token));
    else if (consume(token, "<="))
      node = new_node(ND_LE, node, add(token));
    else if (consume(token, ">"))
      node = new_node(ND_GT, node, add(token));
    else if (consume(token, ">="))
      node = new_node(ND_GE, node, add(token));
    else
      return node;
  }
}

Node *add(Token **token) {
  Node *node = mul(token);

  for (;;) {
    if (consume(token, "+"))
      node = new_node(ND_ADD, node, mul(token));
    else if (consume(token, "-"))
      node = new_node(ND_SUB, node, mul(token));
    else
      return node;
  }
}

Node *mul(Token **token) {
  Node *node = unary(token);

  for (;;) {
    if (consume(token, "*"))
      node = new_node(ND_MUL, node, unary(token));
    else if (consume(token, "/"))
      node = new_node(ND_DIV, node, unary(token));
    else
      return node;
  }
}

Node *unary(Token **token) {
  if (consume(token, "+")) return primary(token);
  if (consume(token, "-"))
    return new_node(ND_SUB, new_node_num(0), primary(token));
  return primary(token);
}

Node *primary(Token **token) {
  // 次のトークンが"("なら、"(" expr ")"のはず
  if (consume(token, "(")) {
    Node *node = expr(token);
    expect(token, ")");
    return node;
  }

  Token *tok = consume_ident(token);
  if (tok) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;

    LVar *lvar = find_lvar(tok);
    if (lvar == NULL) {
      lvar = calloc(1, sizeof(LVar));
      lvar->next = locals;
      lvar->name = tok->str;
      lvar->len = tok->len;
      if (locals == NULL)
        lvar->offset = 0;
      else
        lvar->offset = locals->offset + 1;
      locals = lvar;
    }
    node->offset = lvar->offset;
    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number(token));
}

vector *parse(Token *token) { return program(&token); }

void print_node(Node *node) {
  if (node->kind == ND_NUM) {
    printf("%d", node->val);
  } else if (node->kind == ND_LVAR) {
    printf("%c", 'a' + node->offset);
  } else if (node->kind == ND_ASSIGN) {
    print_node(node->lhs);
    printf("=");
    print_node(node->rhs);
  } else if (node->kind == ND_RETURN) {
    printf("return ");
    print_node(node->lhs);
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
      case ND_EQ:
        printf("==");
        break;
      case ND_NE:
        printf("!=");
        break;
      case ND_LT:
        printf("<");
        break;
      case ND_LE:
        printf("<=");
        break;
      case ND_GT:
        printf(">");
        break;
      case ND_GE:
        printf(">=");
        break;
      case ND_ASSIGN:
        printf("=");
        break;
    }
    print_node(node->rhs);
    printf(")");
  }
}