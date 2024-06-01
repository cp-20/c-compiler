#include "parser.h"

#include "color.h"
#include "tokenizer.h"
#include "vector.h"

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok, vector *locals) {
  for (int i = 0; i < locals->size; i++) {
    LVar *lvar = vec_at(locals, i);
    if (lvar->len == tok->len && !memcmp(tok->str, lvar->name, lvar->len)) {
      return lvar;
    }
  }
  return NULL;
}

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
    Function *func = function(token);
    vec_push_last(code, func);
  }
  return code;
}

static vector *global_locals;

Function *function(Token **token) {
  Function *func = calloc(1, sizeof(Function));

  if (!consume_reserved(token, TK_INT)) {
    error_at((*token)->str, "intではありません");
  }

  Token *tok = consume_ident(token);
  func->name = tok->str;
  func->len = tok->len;

  expect(token, "(");
  vector *locals = new_vector();
  int argc = 0;
  while (!consume(token, ")")) {
    if (!consume_reserved(token, TK_INT)) {
      error_at((*token)->str, "intではありません");
    }

    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    LVar *lvar = find_lvar(tok, locals);
    if (lvar != NULL) {
      error_at(tok->str, "変数が二重定義されています");
    }
    lvar = calloc(1, sizeof(LVar));
    lvar->name = tok->str;
    lvar->len = tok->len;
    lvar->offset = locals->size;
    lvar->var = calloc(1, sizeof(Variable));
    lvar->var->type = TYPE_I32;
    lvar->var->ref_nest = 0;
    vec_push_last(locals, lvar);
    argc++;
    consume(token, ",");
  }
  func->argc = argc;
  func->locals = locals;
  global_locals = locals;

  expect(token, "{");
  vector *stmts = new_vector();
  while (!consume(token, "}")) {
    vec_push_last(stmts, stmt(token));
  }
  func->body = stmts;

  return func;
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

  if (consume_reserved(token, TK_INT)) {
    int ref_nest = 0;
    while (consume(token, "*")) ref_nest++;

    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    LVar *lvar = find_lvar(tok, global_locals);
    if (lvar != NULL) {
      error_at(tok->str, "変数が二重定義されています");
    }
    lvar = calloc(1, sizeof(LVar));
    lvar->name = tok->str;
    lvar->len = tok->len;
    lvar->offset = global_locals->size;
    lvar->var = calloc(1, sizeof(Variable));
    lvar->var->type = TYPE_I32;
    lvar->var->ref_nest = ref_nest;
    vec_push_last(global_locals, lvar);
    expect(token, ";");
    return NULL;
  }

  node = expr(token);
  expect(token, ";");
  return node;
}

Node *expr(Token **token) { return logical(token); }

Node *logical(Token **token) {
  Node *node = assign(token);

  for (;;) {
    if (consume(token, "&&"))
      node = new_node(ND_AND, node, assign(token));
    else if (consume(token, "||"))
      node = new_node(ND_OR, node, assign(token));
    else
      return node;
  }
}

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
  if (consume(token, "!"))
    return new_node(ND_EQ, primary(token), new_node_num(0));
  if (consume(token, "++")) {
    Node *node = primary(token);
    return new_node(ND_ASSIGN, node, new_node(ND_ADD, node, new_node_num(1)));
  }
  if (consume(token, "--")) {
    Node *node = primary(token);
    return new_node(ND_ASSIGN, node, new_node(ND_SUB, node, new_node_num(1)));
  }
  if (consume(token, "&")) return new_node(ND_REF, primary(token), NULL);
  if (consume(token, "*")) return new_node(ND_DEREF, primary(token), NULL);

  Node *node = primary(token);
  if (consume(token, "++")) return new_node(ND_INCR, node, NULL);
  if (consume(token, "--")) return new_node(ND_DECR, node, NULL);
  return node;
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

    if (consume(token, "(")) {
      node->kind = ND_CALL;
      node->call = calloc(1, sizeof(Call));
      node->call->name = tok->str;
      node->call->len = tok->len;
      node->call->args = new_vector();
      while (!consume(token, ")")) {
        vec_push_last(node->call->args, expr(token));
        if (consume(token, ")")) break;
        expect(token, ",");
      }
      return node;
    }

    node->kind = ND_LVAR;
    LVar *lvar = find_lvar(tok, global_locals);
    if (lvar == NULL) {
      lvar = calloc(1, sizeof(LVar));
      lvar->name = tok->str;
      lvar->len = tok->len;
      lvar->offset = global_locals->size;
      lvar->var = calloc(1, sizeof(Variable));
      lvar->var->type = TYPE_I32;
      lvar->var->ref_nest = 0;
      vec_push_last(global_locals, lvar);
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

  if (node->kind == ND_CALL) {
    printf("%.*s(", node->call->len, node->call->name);
    for (int i = 0; i < node->call->args->size; i++) {
      if (i > 0) printf(", ");
      print_node(vec_at(node->call->args, i));
    }
    printf(")");
    return;
  }

  if (node->kind == ND_INCR) {
    print_node(node->lhs);
    printf("++");
    return;
  }

  if (node->kind == ND_DECR) {
    print_node(node->lhs);
    printf("--");
    return;
  }

  if (node->kind == ND_REF) {
    printf("&");
    print_node(node->lhs);
    return;
  }

  if (node->kind == ND_DEREF) {
    printf("*");
    print_node(node->lhs);
    return;
  }

  printf("(");
  print_node(node->lhs);
  if (node->kind == ND_ADD) {
    printf("+");
  } else if (node->kind == ND_SUB) {
    printf("-");
  } else if (node->kind == ND_MUL) {
    printf("*");
  } else if (node->kind == ND_DIV) {
    printf("/");
  } else if (node->kind == ND_EQ) {
    printf("==");
  } else if (node->kind == ND_NE) {
    printf("!=");
  } else if (node->kind == ND_LT) {
    printf("<");
  } else if (node->kind == ND_LE) {
    printf("<=");
  } else if (node->kind == ND_GT) {
    printf(">");
  } else if (node->kind == ND_GE) {
    printf(">=");
  } else if (node->kind == ND_ASSIGN) {
    printf("=");
  } else if (node->kind == ND_AND) {
    printf("&&");
  } else if (node->kind == ND_OR) {
    printf("||");
  }
  print_node(node->rhs);
  printf(")");
}
