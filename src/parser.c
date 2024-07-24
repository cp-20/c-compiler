#include "parser.h"

#include "color.h"
#include "debug.h"
#include "node-helper.h"
#include "tokenizer.h"
#include "vector.h"

void print_debug_token(char *type, Token **token) {
  if (!f_debug) return;
  print_debug(COL_BLUE "[parse] " COL_GREEN "[%s] " COL_YELLOW "%s " COL_RESET
                       "(%d)",
              type, (*token)->str, (*token)->kind);
}

vector *global_locals;
vector *global_structs;
vector *global_typedefs;
vector *global_strings;
vector *global_local_structs;
vector *global_globals;
int anon_structs_index;

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar_from_vector(Token *tok, vector *locals) {
  for (int i = 0; i < locals->size; i++) {
    LVar *lvar = vec_at(locals, i);
    if (lvar->len == tok->len && !memcmp(tok->str, lvar->name, lvar->len)) {
      return lvar;
    }
  }
  return NULL;
}

LVar *find_lvar(Token *tok) {
  LVar *lvar = find_lvar_from_vector(tok, global_locals);
  if (lvar == NULL) {
    lvar = find_lvar_from_vector(tok, global_globals);
  }
  return lvar;
}

Variable *find_struct_from_vector(Token *tok, vector *structs) {
  for (int i = 0; i < structs->size; i++) {
    Variable *var = vec_at(structs, i);
    if (var->len == tok->len && !memcmp(tok->str, var->name, var->len)) {
      return var;
    }
  }
  return NULL;
}

Variable *find_struct(Token *tok) {
  Variable *var = find_struct_from_vector(tok, global_local_structs);
  if (var != NULL) return var;
  var = find_struct_from_vector(tok, global_structs);
  return var;
}

int find_struct_field(Token *tok, Variable *struct_var) {
  print_debug(COL_BLUE "[parse] " COL_GREEN "[find_struct_field] " COL_YELLOW
                       "%.*s" COL_RESET,
              tok->len, tok->str);
  for (int i = 0; i < struct_var->fields->size; i++) {
    LVar *field = vec_at(struct_var->fields, i);
    if (field->len == tok->len && !memcmp(tok->str, field->name, field->len)) {
      return i;
    }
  }
  return -1;
}

Variable *find_typedef(Token *tok) {
  for (int i = 0; i < global_typedefs->size; i++) {
    Variable *var = vec_at(global_typedefs, i);
    if (var->len == tok->len && !memcmp(tok->str, var->name, var->len)) {
      return var;
    }
  }
  return NULL;
}

Program *program(Token **token) {
  print_debug_token("program", token);

  vector *code = new_vector();
  global_structs = new_vector();
  global_globals = new_vector();
  global_typedefs = new_vector();
  global_strings = new_vector();
  anon_structs_index = 0;
  while ((*token)->kind != TK_EOF) {
    global_local_structs = new_vector();
    Function *func = global_decl(token);
    if (func == NULL) continue;
    vec_push_last(code, func);
    func->structs = global_local_structs;
  }
  Program *program = calloc(1, sizeof(Program));
  program->functions = code;
  program->structs = global_structs;
  program->globals = global_globals;
  program->typedefs = global_typedefs;
  program->strings = global_strings;
  return program;
}

Function *global_decl(Token **token) {
  print_debug_token("global_decl", token);

  if (consume_reserved(token, TK_EXTERN)) {
    Variable *return_type = type(token, false);
    if (return_type == NULL) {
      error_at((*token)->str, "型ではありません");
    }
    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->name = tok->str;
    lvar->len = tok->len;
    lvar->offset = -global_globals->size - 1;
    lvar->var = return_type;
    lvar->var->name = tok->str;
    lvar->var->len = tok->len;
    lvar->var->reg = -2;
    vec_push_last(global_globals, lvar);
    expect(token, ";");
    return NULL;
  }

  if (enum_decl(token)) {
    expect(token, ";");
    return NULL;
  }

  bool f_typedef = false;
  if (consume_reserved(token, TK_TYPEDEF)) {
    f_typedef = true;
  }

  if (f_typedef && enum_decl(token)) {
    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    expect(token, ";");

    Variable *var = find_typedef(tok);
    var = new_variable(-1, TYPE_I32, NULL, 0);
    var->name = tok->str;
    var->len = tok->len;
    vec_push_last(global_typedefs, var);

    return NULL;
  }

  Variable *return_type = type(token, false);
  if (return_type != NULL) {
    if (return_type->type == TYPE_STRUCT) {
      vec_push_last(global_structs, return_type);
    }

    Token *tok = consume_ident(token);
    if (tok == NULL) {
      if (f_typedef) {
        error_at((*token)->str, "識別子ではありません");
      }
      expect(token, ";");
      return NULL;
    }

    if (f_typedef) {
      Variable *var = find_typedef(tok);
      if (var != NULL) {
        error_at(tok->str, "変数が二重定義されています");
      }
      var = return_type;
      var->name = tok->str;
      var->len = tok->len;
      vec_push_last(global_typedefs, var);
      expect(token, ";");
      return NULL;
    }

    // 関数の定義
    if (consume(token, "(")) {
      Function *func = calloc(1, sizeof(Function));
      func->name = tok->str;
      func->len = tok->len;
      func->ret = return_type;

      vector *locals = new_vector();
      int argc = 0;
      while (!consume(token, ")")) {
        Variable *argument_type = type(token, false);
        if (argument_type == NULL) {
          error_at((*token)->str, "型ではありません");
        }

        Token *tok = consume_ident(token);
        if (tok == NULL) {
          error_at((*token)->str, "識別子ではありません");
        }
        LVar *lvar = find_lvar_from_vector(tok, locals);
        if (lvar != NULL) {
          error_at(tok->str, "変数が二重定義されています");
        }
        lvar = calloc(1, sizeof(LVar));
        lvar->name = tok->str;
        lvar->len = tok->len;
        lvar->offset = locals->size;
        lvar->var = argument_type;
        vec_push_last(locals, lvar);
        argc++;
        consume(token, ",");
      }
      func->argc = argc;
      func->locals = locals;
      global_locals = locals;
      if (consume(token, "{")) {
        vector *stmts = new_vector();
        while (!consume(token, "}")) {
          vec_push_last(stmts, stmt(token));
        }
        func->body = stmts;
        return func;
      } else {
        // プロトタイプ宣言なら何もしない
        expect(token, ";");
        return NULL;
      }
    }

    // グローバル変数の宣言
    if (consume(token, ";")) {
      LVar *lvar = find_lvar_from_vector(tok, global_globals);
      if (lvar != NULL) {
        error_at(tok->str, "変数が二重定義されています");
      }
      lvar = calloc(1, sizeof(LVar));
      lvar->name = tok->str;
      lvar->len = tok->len;
      lvar->offset = -global_globals->size - 1;
      lvar->var = return_type;
      lvar->var->name = tok->str;
      lvar->var->len = tok->len;
      lvar->var->reg = -1;
      vec_push_last(global_globals, lvar);
      return NULL;
    }
  }

  error_at((*token)->str, "関数または構造体の定義ではありません");
  return NULL;
}

Node *stmt(Token **token) {
  print_debug_token("stmt", token);

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

Node *expr(Token **token) { return local_decl(token); }

Node *local_decl(Token **token) {
  print_debug_token("local_decl", token);

  Variable *var_type = type(token, true);
  if (var_type == NULL) return ternary(token);

  Node *node = new_node(ND_GROUP, NULL, NULL);
  vector *stmts = new_vector();
  do {
    int ref_nest = 0;
    while (consume(token, "*")) ref_nest++;

    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    LVar *lvar = find_lvar(tok);
    if (lvar != NULL) {
      error_at(tok->str, "変数が二重定義されています");
    }
    lvar = calloc(1, sizeof(LVar));
    lvar->name = tok->str;
    lvar->len = tok->len;
    lvar->offset = global_locals->size;

    Variable *var = var_type;
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
          Node *val = ternary(token);
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

        if (consume(token, "{")) {
          int array_element = 0;
          while (!consume(token, "}")) {
            Node *val = ternary(token);
            Node *lvar_index_node =
                new_node(ND_ACCESS, lvar_node, new_node_num(array_element));
            Node *assign_node = new_node(ND_ASSIGN, lvar_index_node, val);
            vec_push_last(stmts, assign_node);
            array_element++;
            if (consume(token, "}")) break;
            expect(token, ",");
          }
        } else {
          Node *init_node = new_node(ND_ASSIGN, lvar_node, expr(token));
          vec_push_last(stmts, init_node);
        }
      }
    }
  } while (consume(token, ","));
  node->stmts = stmts;
  return node;
}

Node *ternary(Token **token) {
  print_debug_token("ternary", token);

  Node *node = logical(token);
  if (consume(token, "?")) {
    Node *lhs = logical(token);
    expect(token, ":");
    Node *rhs = ternary(token);
    Node *result = new_node(ND_TERNARY, node, new_node(ND_GROUP, lhs, rhs));
    // とりあえず左辺の型にする (ホントは一致しているか確かめた方が良い気がする)
    result->cast = get_node_type(lhs, global_locals);
    return result;
  }
  return node;
}

Node *logical(Token **token) {
  print_debug_token("logical", token);

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
  print_debug("node->offset = %d", node->offset);
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
  print_debug_token("unary", token);

  if (consume_reserved(token, TK_SIZEOF)) {
    expect(token, "(");
    Variable *node_type = type(token, false);
    if (node_type == NULL) {
      Node *node = logical(token);
      node_type = get_node_type(node, global_locals);
    }
    expect(token, ")");
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
  print_debug_token("primary", token);

  if (consume(token, "(")) {
    Variable *cast_type = type(token, false);
    if (cast_type != NULL) {
      expect(token, ")");
      Node *node = primary(token);
      node->cast = cast_type;
      return node;
    }
    Node *node = expr(token);
    expect(token, ")");

    node = parse_primary_access(token, node);
    return node;
  }

  Token *tok = consume_ident(token);
  if (tok != NULL) {
    Node *node = calloc(1, sizeof(Node));

    if (consume(token, "(")) {
      node->kind = ND_CALL;
      node->call = calloc(1, sizeof(Call));
      node->call->name = tok->str;
      node->call->len = tok->len;
      node->call->ret = new_variable(-1, TYPE_I32, NULL, 0);
      node->call->args = new_vector();
      while (!consume(token, ")")) {
        vec_push_last(node->call->args, expr(token));
        if (consume(token, ")")) break;
        expect(token, ",");
      }
    } else {
      node->kind = ND_LVAR;
      LVar *lvar = find_lvar(tok);
      if (lvar == NULL) {
        error_at(tok->str, "未定義の変数です: %.*s", tok->len, tok->str);
      }
      node->offset = lvar->offset;
    }

    node = parse_primary_access(token, node);
    return node;
  }

  tok = *token;
  if (consume_reserved(token, TK_STRING)) {
    Token *next_tok = *token;
    while (consume_reserved(token, TK_STRING)) {
      Token *new_tok = calloc(1, sizeof(Token));
      new_tok->str = calloc(1, next_tok->len + tok->len);
      new_tok->len = sprintf(new_tok->str, "%.*s%.*s", tok->len, tok->str,
                             next_tok->len, next_tok->str);
      tok = new_tok;
      next_tok = *token;
    }
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_STRING;
    node->offset = global_strings->size;
    vec_push_last(global_strings, tok);
    return node;
  }

  return new_node_num(expect_number(token));
}

Node *parse_primary_access(Token **token, Node *node) {
  print_debug_token("parse_primary_access", token);

  while (true) {
    if (consume(token, "[")) {
      Node *index_node = logical(token);
      node = new_node(ND_DEREF, new_node(ND_ADD, node, index_node), NULL);
      expect(token, "]");
      continue;
    }
    if (consume(token, ".")) {
      Token *tok = consume_ident(token);
      if (tok == NULL) {
        error_at((*token)->str, "アクセス先のメンバーが存在しません");
        return NULL;
      }
      Variable *struct_var = get_node_type(node, global_locals);
      int field_index = find_struct_field(tok, struct_var);
      if (field_index == -1) {
        error_at(tok->str, "メンバーが存在しません");
        return NULL;
      }
      node = new_node(ND_ACCESS, node, new_node_num(field_index));
      continue;
    }
    if (consume(token, "->")) {
      Token *tok = consume_ident(token);
      if (tok == NULL) {
        error_at((*token)->str, "アクセス先のメンバーが存在しません");
        return NULL;
      }
      Variable *struct_var = get_node_type(node, global_locals);
      int field_index = find_struct_field(tok, struct_var->ptr_to);
      if (field_index == -1) {
        error_at(tok->str, "メンバーが存在しません");
        return NULL;
      }
      Node *struct_node = new_node(ND_DEREF, node, NULL);
      node = new_node(ND_ACCESS, struct_node, new_node_num(field_index));
      continue;
    }
    break;
  }
  return node;
}

Variable *parse_struct(Token **token, bool is_declare) {
  print_debug_token("parse_struct", token);

  if (!consume_reserved(token, TK_STRUCT)) {
    return NULL;
  }
  Token *tok = consume_ident(token);
  if (tok == NULL) {
    if (is_declare) {
      error_at((*token)->str, "識別子ではありません");
      return NULL;
    }
    tok = calloc(1, sizeof(Token));
    tok->str = calloc(1, 16);
    sprintf(tok->str, "anon.%d", anon_structs_index++);
    tok->len = strlen(tok->str);
  }
  Variable *var = find_struct(tok);

  if (consume(token, "{")) {
    if (var != NULL) {
      error_at(tok->str, "変数が二重定義されています");
      return NULL;
    }
    var = calloc(1, sizeof(Variable));
    var->type = TYPE_STRUCT;
    var->fields = new_vector();
    var->name = tok->str;
    var->len = tok->len;
    if (!is_declare) {
      vec_push_last(global_local_structs, var);
    }

    while (!consume(token, "}")) {
      Variable *field_type = type(token, false);
      if (field_type == NULL) {
        error_at((*token)->str, "型ではありません");
        return NULL;
      }
      Token *tok = consume_ident(token);
      if (tok == NULL) {
        error_at((*token)->str, "識別子ではありません");
        return NULL;
      }
      LVar *field = calloc(1, sizeof(LVar));
      field->name = tok->str;
      field->len = tok->len;
      field->var = field_type;
      vec_push_last(var->fields, field);
      expect(token, ";");
    }
  } else {
    if (var == NULL) {
      error_at(tok->str, "未定義の構造体です: %.*s", tok->len, tok->str);
      return NULL;
    }
  }
  return var;
}

bool enum_decl(Token **token) {
  if (consume_reserved(token, TK_ENUM)) {
    expect(token, "{");
    int enum_value = 0;
    while (!consume(token, "}")) {
      Token *tok = consume_ident(token);
      if (tok == NULL) {
        error_at((*token)->str, "識別子ではありません");
      }
      LVar *lvar = find_lvar_from_vector(tok, global_globals);
      if (lvar != NULL) {
        error_at(tok->str, "変数が二重定義されています");
      }
      lvar = calloc(1, sizeof(LVar));
      lvar->name = tok->str;
      lvar->len = tok->len;
      lvar->offset = -global_globals->size - 1;
      lvar->var = new_variable(-1, TYPE_I32, NULL, 0);
      lvar->var->value = malloc(sizeof(int));
      *lvar->var->value = enum_value;
      vec_push_last(global_globals, lvar);
      enum_value++;
      consume(token, ",");
    }
    return true;
  }
  return false;
}

Variable *type(Token **token, bool exclude_ptr) {
  print_debug_token("type", token);

  Variable *var = NULL;
  int type = -1;
  if (consume_reserved(token, TK_VOID)) {
    type = TYPE_VOID;
  } else if (consume_reserved(token, TK_INT)) {
    type = TYPE_I32;
  } else if (consume_reserved(token, TK_CHAR)) {
    type = TYPE_I8;
  }
  if (type != -1) {
    var = new_variable(-1, type, NULL, 0);
  }

  if (var == NULL) {
    var = find_typedef(*token);
    if (var != NULL) consume_ident(token);
  }

  if (var == NULL) {
    var = parse_struct(token, false);
  }

  if (!exclude_ptr) {
    int ref_nest = 0;
    while (consume(token, "*")) ref_nest++;
    for (int nest = ref_nest; nest > 0; nest--) {
      Variable *ptr = new_variable(-1, TYPE_PTR, var, 0);
      var = ptr;
    }
  }

  return var;
}

Program *parse(Token *token) { return program(&token); }
