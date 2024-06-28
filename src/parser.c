#include "parser.h"

#include "color.h"
#include "node-helper.h"
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

  node = expr(token);
  expect(token, ";");
  return node;
}

Node *expr(Token **token) { return declaration(token); }

Node *declaration(Token **token) {
  if (!consume_reserved(token, TK_INT)) return logical(token);

  int ref_nest = 0;
  while (consume(token, "*")) ref_nest++;

  Node *node = new_node(ND_GROUP, NULL, NULL);
  vector *stmts = new_vector();
  do {
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

    Variable *var = new_variable(-1, TYPE_I32, NULL, 0);
    for (int nest = ref_nest; nest > 0; nest--) {
      Variable *ptr = new_variable(-1, TYPE_PTR, var, 0);
      var = ptr;
    }
    lvar->var = var;

    if (consume(token, "[")) {
      int array_size = consume_number(token);
      lvar->var = new_variable(-1, TYPE_ARRAY, lvar->var, array_size);
      expect(token, "]");
      vec_push_last(global_locals, lvar);

      if (consume(token, "=")) {
        Node *lvar_node = new_node(ND_LVAR, NULL, NULL);
        lvar_node->offset = lvar->offset;

        expect(token, "{");
        int array_element = 0;
        while (!consume(token, "}")) {
          Node *val = logical(token);
          Node *lvar_index_node =
              new_node(ND_ADD, lvar_node, new_node_num(array_element));
          Node *lvar_deref_node = new_node(ND_DEREF, lvar_index_node, NULL);
          Node *assign_node = new_node(ND_ASSIGN, lvar_deref_node, val);
          vec_push_last(stmts, assign_node);
          array_element++;
          if (consume(token, "}")) break;
          expect(token, ",");
        }
        if (array_size >= 0 && array_size != array_element) {
          error_at(tok->str, "配列の宣言サイズと初期化サイズが一致しません");
        }
        if (array_size < 0) {
          lvar->var->array_size = array_element;
        }
      } else {
        if (array_size < 0) {
          error_at(tok->str, "配列のサイズが指定されていません");
        }
      }
    } else {
      vec_push_last(global_locals, lvar);

      if (consume(token, "=")) {
        Node *lvar_node = new_node(ND_LVAR, NULL, NULL);
        lvar_node->offset = lvar->offset;
        Node *init_node = new_node(ND_ASSIGN, lvar_node, expr(token));
        vec_push_last(stmts, init_node);
      }
    }
  } while (consume(token, ","));
  node->stmts = stmts;
  return node;
}

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
  if (consume_reserved(token, TK_SIZEOF)) {
    Node *node = unary(token);
    Variable *node_type = get_node_type(node, global_locals);
    int node_size = get_variable_size(node_type);
    return new_node_num(node_size);
  }
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
  bool is_ref = consume(token, "&");
  bool is_deref = consume(token, "*");
  if (is_ref || is_deref) {
    int ref_nest = 1;
    if (is_deref) {
      ref_nest = -1;
    }
    while (1) {
      if (consume(token, "&")) {
        ref_nest++;
      } else if (consume(token, "*")) {
        ref_nest--;
      } else {
        break;
      }
    }
    Node *node = primary(token);
    while (ref_nest > 0) {
      node = new_node(ND_REF, node, NULL);
      ref_nest--;
    }
    while (ref_nest < 0) {
      node = new_node(ND_DEREF, node, NULL);
      ref_nest++;
    }
    return node;
  }

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
      error_at(tok->str, "未定義の変数です: %.*s", tok->len, tok->str);
    }
    node->offset = lvar->offset;

    if (consume(token, "[")) {
      int index = expect_number(token);
      node =
          new_node(ND_DEREF, new_node(ND_ADD, node, new_node_num(index)), NULL);
      expect(token, "]");
    }

    return node;
  }

  // そうでなければ数値のはず
  return new_node_num(expect_number(token));
}

vector *parse(Token *token) { return program(&token); }
