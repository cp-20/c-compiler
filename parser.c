#include "parser.h"

#include "color.h"
#include "tokenizer.h"
#include "vector.h"

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

  if (consume(token, "{")) {
    vector *stmts = new_vector();
    while (!consume(token, "}")) {
      vec_push_last(stmts, stmt(token));
    }
    node = new_node(ND_BLOCK, NULL, NULL);
    node->stmts = stmts;
    return node;
  }

  if (consume_reserved(token, TK_RETURN)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_RETURN;
    node->lhs = expr(token);
    expect(token, ";");
    return node;
  }

  if (consume_reserved(token, TK_IF)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_IF;
    expect(token, "(");
    node->lhs = expr(token);
    expect(token, ")");
    node->rhs = stmt(token);
    if (consume_reserved(token, TK_ELSE)) {
      node->extra = stmt(token);
    }
    return node;
  }

  if (consume_reserved(token, TK_WHILE)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_WHILE;
    expect(token, "(");
    node->lhs = expr(token);
    expect(token, ")");
    node->rhs = stmt(token);
    return node;
  }

  if (consume_reserved(token, TK_FOR)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_FOR;
    expect(token, "(");
    if (consume(token, ";")) {
      node->lhs = NULL;
    } else {
      node->lhs = expr(token);
      expect(token, ";");
    }
    if (consume(token, ";")) {
      node->rhs = NULL;
    } else {
      node->rhs = expr(token);
      expect(token, ";");
    }
    if (consume(token, ")")) {
      node->extra = NULL;
    } else {
      node->extra = expr(token);
      expect(token, ")");
    }

    node->extra2 = stmt(token);

    return node;
  }

  node = expr(token);
  expect(token, ";");
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
  if (node == NULL) return;

  if (node->kind == ND_NUM) {
    printf("%d", node->val);
    return;
  }

  if (node->kind == ND_LVAR) {
    printf("%c", 'a' + node->offset);
    if (node->kind == ND_ASSIGN) {
      print_node(node->lhs);
      printf("=");
      print_node(node->rhs);
    }
    return;
  }

  if (node->kind == ND_RETURN) {
    printf("return ");
    print_node(node->lhs);
    return;
  }

  if (node->kind == ND_IF) {
    printf("if (");
    print_node(node->lhs);
    printf(") ");
    print_node(node->rhs);
    if (node->extra) {
      printf(" else ");
      print_node(node->extra);
    }
    return;
  }

  if (node->kind == ND_WHILE) {
    printf("while (");
    print_node(node->lhs);
    printf(") ");
    print_node(node->rhs);
    return;
  }

  if (node->kind == ND_FOR) {
    printf("for (");
    if (node->lhs) print_node(node->lhs);
    printf(";");
    if (node->rhs) print_node(node->rhs);
    printf(";");
    if (node->extra) print_node(node->extra);
    printf(")");
    return;
  }

  if (node->kind == ND_BLOCK) {
    printf("{ ");
    for (int i = 0; i < node->stmts->size; i++) {
      print_node(vec_at(node->stmts, i));
      printf("; ");
    }
    printf("}");
    return;
  }

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