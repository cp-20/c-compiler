#include "parser.h"

#include "color.h"
#include "debug.h"
#include "node-helper.h"
#include "tokenizer.h"
#include "vector.h"

void print_debug_token(char *type, Token **token) {
  if (true) return;
  int len = (*token)->len > 0 ? (*token)->len : 1;
  char *line = (*token)->str;
  char *line_start = line;
  while (user_input < line_start && line_start[-1] != '\n') line_start--;
  char *line_end = line;
  while (*line_end != '\n') line_end++;
  print_debug(COL_BLUE "[parse] " COL_GREEN "[%s] " COL_RESET "%.*s" COL_YELLOW
                       "%.*s" COL_RESET "%.*s" COL_CYAN " (%d)" COL_RESET,
              type, line - line_start, line_start, len, line,
              line_end - line - len, line + len, (*token)->kind);
}

vector *global_locals;
vector *global_structs;
vector *global_typedefs;
vector *global_strings;
vector *global_local_structs;
vector *global_globals;
vector *global_functions;
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
  LVar *lvar;
  for (int i = 0; i < global_locals->size; i++) {
    vector *locals = vec_at(global_locals, i);
    lvar = find_lvar_from_vector(tok, locals);
    if (lvar != NULL) return lvar;
  }

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
    if (field->len == tok->len &&
        memcmp(tok->str, field->name, field->len) == 0) {
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

int get_offset() {
  int offset = 0;
  for (int i = 0; i < global_locals->size; i++) {
    vector *locals = vec_at(global_locals, i);
    offset += locals->size;
  }
  return offset;
}

Function *get_function(Token *tok) {
  for (int i = 0; i < global_functions->size; i++) {
    Function *func = vec_at(global_functions, i);
    if (func->len == tok->len && !memcmp(tok->str, func->name, func->len)) {
      return func;
    }
  }
  return NULL;
}

void setup_program() {
  Variable *void_ptr =
      new_variable(-1, TYPE_PTR, new_variable(-1, TYPE_VOID, NULL, 0), 0);

  // stderr
  LVar *var_stderr = calloc(1, sizeof(LVar));
  var_stderr->name = calloc(7, sizeof(char));
  sprintf(var_stderr->name, "stderr");
  var_stderr->len = 6;
  var_stderr->offset = -global_globals->size - 1;
  var_stderr->var = copy_var(void_ptr);
  var_stderr->var->name = calloc(7, sizeof(char));
  sprintf(var_stderr->var->name, "stderr");
  var_stderr->var->len = 6;
  var_stderr->var->reg = -2;
  vec_push_last(global_globals, var_stderr);

  // va_list
  Variable *__va_list_tag = new_variable(-1, TYPE_STRUCT, NULL, 0);
  __va_list_tag->name = calloc(14, sizeof(char));
  sprintf(__va_list_tag->name, "__va_list_tag");
  __va_list_tag->len = 13;
  __va_list_tag->fields = new_vector();
  LVar *i32 = calloc(1, sizeof(LVar));
  i32->var = new_variable(-1, TYPE_I32, NULL, 0);
  vec_push_last(__va_list_tag->fields, i32);
  vec_push_last(__va_list_tag->fields, i32);
  LVar *ptr = calloc(1, sizeof(LVar));
  ptr->var = copy_var(void_ptr);
  vec_push_last(__va_list_tag->fields, ptr);
  vec_push_last(__va_list_tag->fields, ptr);
  vec_push_last(global_structs, __va_list_tag);
  Variable *var_va_list =
      new_variable(-1, TYPE_ARRAY, copy_var(__va_list_tag), 1);
  var_va_list->name = calloc(8, sizeof(char));
  sprintf(var_va_list->name, "va_list");
  var_va_list->len = 7;
  vec_push_last(global_typedefs, var_va_list);

  // bool
  Variable *var_bool = new_variable(-1, TYPE_I32, NULL, 0);
  var_bool->name = calloc(5, sizeof(char));
  sprintf(var_bool->name, "bool");
  var_bool->len = 4;
  vec_push_last(global_typedefs, var_bool);

  free_variable(void_ptr);

  // FILE
  Variable *var_FILE = new_variable(-1, TYPE_STRUCT, NULL, 0);
  var_FILE->name = calloc(5, sizeof(char));
  sprintf(var_FILE->name, "FILE");
  var_FILE->len = 4;
  vec_push_last(global_typedefs, var_FILE);
}

char *function_conversion(char *name) {
  if (strcmp(name, "va_end") == 0) {
    char *alt = calloc(12, sizeof(char));
    sprintf(alt, "llvm.va_end");
    free(name);
    return alt;
  }
  if (strcmp(name, "va_start") == 0) {
    char *alt = calloc(14, sizeof(char));
    sprintf(alt, "llvm.va_start");
    free(name);
    return alt;
  }
  if (strcmp(name, "scanf") == 0) {
    char *alt = calloc(15, sizeof(char));
    sprintf(alt, "__isoc99_scanf");
    free(name);
    return alt;
  }
  return name;
}

Program *program(Token **token) {
  print_debug_token("program", token);

  vector *code = new_vector();
  global_structs = new_vector();
  global_globals = new_vector();
  global_typedefs = new_vector();
  global_strings = new_vector();
  global_functions = new_vector();
  anon_structs_index = 0;
  setup_program();
  while ((*token)->kind != TK_EOF) {
    global_local_structs = new_vector();
    Function *func = global_decl(token);
    if (func == NULL) continue;
    for (int i = 0; i < code->size; i++) {
      Function *f = vec_at(code, i);
      if (f->len == func->len && !memcmp(f->name, func->name, f->len)) {
        if (f->is_proto) {
          vec_remove(code, i);
          break;
        }
        error_at((*token)->str, "関数が二重定義されています");
      }
    }

    vec_push_last(code, func);
    func->structs = global_local_structs;
  }
  Program *program = calloc(1, sizeof(Program));
  program->functions = code;
  program->structs = global_structs;
  program->globals = global_globals;
  program->typedefs = global_typedefs;
  program->strings = global_strings;
  vec_free(global_functions);

  return program;
}

Function *global_decl(Token **token) {
  print_debug_token("global_decl", token);

  if (consume_reserved(token, TK_EXTERN)) {
    Variable *return_type = type(token, false, false);
    if (return_type == NULL) {
      error_at((*token)->str, "型ではありません");
    }
    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    LVar *lvar = find_lvar_from_vector(tok, global_globals);
    if (lvar == NULL) {
      LVar *lvar = calloc(1, sizeof(LVar));
      lvar->name = calloc(tok->len + 1, sizeof(char));
      memcpy(lvar->name, tok->str, tok->len);
      lvar->len = tok->len;
      lvar->offset = -global_globals->size - 1;
      lvar->var = return_type;
      lvar->var->name = calloc(tok->len + 1, sizeof(char));
      memcpy(lvar->var->name, tok->str, tok->len);
      lvar->var->len = tok->len;
      lvar->var->reg = -2;
      vec_push_last(global_globals, lvar);
    }
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
    var->name = calloc(tok->len + 1, sizeof(char));
    memcpy(var->name, tok->str, tok->len);
    var->len = tok->len;
    vec_push_last(global_typedefs, var);

    return NULL;
  }

  Variable *return_type = type(token, false, f_typedef);
  if (return_type != NULL) {
    if (return_type->type == TYPE_STRUCT) {
      Token *tok = calloc(1, sizeof(Token));
      tok->kind = TK_IDENT;
      tok->str = return_type->name;
      tok->len = return_type->len;
      Variable *st = find_struct_from_vector(tok, global_structs);
      if (st == NULL) vec_push_last(global_structs, return_type);
      free(tok);
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
      var->name = calloc(tok->len + 1, sizeof(char));
      memcpy(var->name, tok->str, tok->len);
      var->len = tok->len;
      vec_push_last(global_typedefs, var);
      expect(token, ";");
      return NULL;
    }

    // 関数の定義
    if (consume(token, "(")) {
      Function *func = calloc(1, sizeof(Function));
      func->name = calloc(tok->len + 1, sizeof(char));
      memcpy(func->name, tok->str, tok->len);
      func->len = tok->len;
      func->ret = return_type;
      func->have_va_arg = false;

      vec_push_last(global_functions, func);

      vector *locals = new_vector();
      int argc = 0;
      if ((*token)->kind == TK_VOID && (*token)->next->kind == TK_RESERVED &&
          (*token)->next->str[0] == ')') {
        consume_reserved(token, TK_VOID);
        expect(token, ")");
      } else {
        while (!consume(token, ")")) {
          if (consume(token, "...")) {
            func->have_va_arg = true;
            expect(token, ")");
            break;
          }
          Variable *argument_type = type(token, false, false);
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
          lvar->name = calloc(tok->len + 1, sizeof(char));
          memcpy(lvar->name, tok->str, tok->len);
          lvar->len = tok->len;
          lvar->offset = locals->size;
          lvar->var = argument_type;
          vec_push_last(locals, lvar);
          argc++;
          consume(token, ",");
        }
      }
      func->argc = argc;
      func->locals = locals;
      global_locals = new_vector();
      vec_push_first(global_locals, locals);
      if (consume(token, "{")) {
        func->is_proto = false;
        vector *stmts = new_vector();
        while (!consume(token, "}")) {
          vec_push_last(stmts, stmt(token));
        }
        func->body = stmts;
      } else {
        func->is_proto = true;
        expect(token, ";");
      }
      vec_free(global_locals);
      return func;
    }

    // グローバル変数の宣言
    if (consume(token, ";")) {
      LVar *lvar = find_lvar_from_vector(tok, global_globals);
      if (lvar != NULL) {
        if (lvar->var->reg != -2) {
          error_at(tok->str, "変数が二重定義されています");
        } else {
          lvar->var->reg = -1;
          return NULL;
        }
      }
      lvar = calloc(1, sizeof(LVar));
      lvar->name = calloc(tok->len + 1, sizeof(char));
      memcpy(lvar->name, tok->str, tok->len);
      lvar->len = tok->len;
      lvar->offset = -global_globals->size - 1;
      lvar->var = return_type;
      lvar->var->name = calloc(tok->len + 1, sizeof(char));
      memcpy(lvar->var->name, tok->str, tok->len);
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
    vector *locals = new_vector();
    vec_push_first(global_locals, locals);
    vector *stmts = new_vector();
    while (!consume(token, "}")) {
      vec_push_last(stmts, stmt(token));
    }
    vec_shift(global_locals);
    node = new_node(ND_BLOCK, NULL, NULL);
    node->stmts = stmts;
    node->locals = locals;
    return node;
  }

  if (consume_reserved(token, TK_RETURN)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_RETURN;
    if (consume(token, ";")) {
      node->lhs = NULL;
    } else {
      node->lhs = expr(token);
      expect(token, ";");
    }
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

  if (consume_reserved(token, TK_DO)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_DO;
    node->lhs = stmt(token);
    expect_reserved(token, TK_WHILE);
    expect(token, "(");
    node->rhs = expr(token);
    expect(token, ")");
    expect(token, ";");
    return node;
  }

  if (consume_reserved(token, TK_FOR)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_FOR;
    node->locals = new_vector();
    vec_push_first(global_locals, node->locals);
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

    vec_shift(global_locals);

    return node;
  }

  if (consume_reserved(token, TK_CONTINUE)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_CONTINUE;
    expect(token, ";");
    return node;
  }

  if (consume_reserved(token, TK_BREAK)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_BREAK;
    expect(token, ";");
    return node;
  }

  if (consume_reserved(token, TK_SWITCH)) {
    node = calloc(1, sizeof(Node));
    node->kind = ND_SWITCH;
    expect(token, "(");
    node->lhs = expr(token);
    expect(token, ")");
    expect(token, "{");
    vector *cases = new_vector();
    while (!consume(token, "}")) {
      bool is_case = consume_reserved(token, TK_CASE);
      if (!is_case) {
        if (!consume_reserved(token, TK_DEFAULT)) {
          error_at((*token)->str, "caseまたはdefaultではありません");
        }
      }

      Node *case_node = calloc(1, sizeof(Node));
      case_node->kind = is_case ? ND_CASE : ND_DEFAULT;
      if (is_case) {
        case_node->lhs = constant(token);
      }
      expect(token, ":");

      vector *stmts = new_vector();
      if (consume(token, "{")) {
        vector *locals = new_vector();
        vec_push_first(global_locals, locals);
        while (!consume(token, "}")) {
          vec_push_last(stmts, stmt(token));
        }
        vec_shift(global_locals);
        case_node->rhs = new_node(ND_BLOCK, NULL, NULL);
        case_node->rhs->locals = locals;
      } else {
        while ((*token)->kind != TK_CASE && (*token)->kind != TK_DEFAULT &&
               !((*token)->kind == TK_RESERVED && (*token)->str[0] == '}' &&
                 (*token)->len == 1)) {
          vec_push_last(stmts, stmt(token));
        }
        case_node->rhs = new_node(ND_GROUP, NULL, NULL);
      }
      case_node->rhs->stmts = stmts;
      if (is_case) {
        vec_push_last(cases, case_node);
      } else {
        if (node->rhs != NULL) {
          error_at((*token)->str, "defaultは一度しか使えません");
        }
        node->rhs = case_node;
      }
    }
    node->stmts = cases;

    return node;
  }

  node = expr(token);
  expect(token, ";");
  return node;
}

Node *expr(Token **token) { return local_decl(token); }

Node *local_decl(Token **token) {
  print_debug_token("local_decl", token);

  Variable *var_type = type(token, true, false);
  if (var_type == NULL) return ternary(token);

  Node *node = new_node(ND_GROUP, NULL, NULL);
  vector *stmts = new_vector();
  vector *locals = vec_first(global_locals);
  do {
    int ref_nest = 0;
    while (consume(token, "*")) ref_nest++;

    Token *tok = consume_ident(token);
    if (tok == NULL) {
      error_at((*token)->str, "識別子ではありません");
    }
    LVar *lvar = find_lvar_from_vector(tok, locals);
    if (lvar != NULL) {
      error_at(tok->str, "変数が二重定義されています");
    }
    lvar = calloc(1, sizeof(LVar));
    lvar->name = calloc(tok->len + 1, sizeof(char));
    memcpy(lvar->name, tok->str, tok->len);
    lvar->len = tok->len;
    lvar->offset = get_offset();

    Variable *var = copy_var_if_needed(var_type);
    for (int nest = ref_nest; nest > 0; nest--) {
      Variable *ptr = new_variable(-1, TYPE_PTR, var, 0);
      var = ptr;
    }
    lvar->var = var;

    if (consume(token, "[")) {
      int array_size = consume_number(token);
      lvar->var = new_variable(-1, TYPE_ARRAY, lvar->var, array_size);
      expect(token, "]");
      vec_push_last(locals, lvar);

      if (consume(token, "=")) {
        Node *lvar_node = new_node(ND_LVAR, NULL, NULL);
        lvar_node->offset = lvar->offset;

        expect(token, "{");
        int array_element = 0;
        while (!consume(token, "}")) {
          Node *val = ternary(token);
          Node *lvar_index_node = new_node(ND_ADD, copy_node(lvar_node),
                                           new_node_num(array_element));
          Node *lvar_deref_node = new_node(ND_DEREF, lvar_index_node, NULL);
          Node *assign_node = new_node(ND_ASSIGN, lvar_deref_node, val);
          vec_push_last(stmts, assign_node);
          array_element++;
          if (consume(token, "}")) break;
          expect(token, ",");
        }
        free_node(lvar_node);
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
      print_debug("push local variable: %.*s (offset = %d)", lvar->len,
                  lvar->name, lvar->offset);
      print_debug("type = %s", get_variable_type_str(lvar->var));
      vec_push_last(locals, lvar);

      if (consume(token, "=")) {
        Node *lvar_node = new_node(ND_LVAR, NULL, NULL);
        lvar_node->offset = lvar->offset;

        if (consume(token, "{")) {
          int array_element = 0;
          while (!consume(token, "}")) {
            Node *val = ternary(token);
            Node *lvar_index_node = new_node(ND_ACCESS, copy_node(lvar_node),
                                             new_node_num(array_element));
            Node *assign_node = new_node(ND_ASSIGN, lvar_index_node, val);
            vec_push_last(stmts, assign_node);
            array_element++;
            if (consume(token, "}")) break;
            expect(token, ",");
          }
          free_node(lvar_node);
        } else {
          Node *init_node = new_node(ND_ASSIGN, lvar_node, expr(token));
          vec_push_last(stmts, init_node);
        }
      }
    }
  } while (consume(token, ","));
  free_variable(var_type);
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
  if (consume(token, "=")) node = new_node(ND_ASSIGN, node, assign(token));
  if (consume(token, "+="))
    node = new_node(ND_ASSIGN, node,
                    new_node(ND_ADD, copy_node(node), assign(token)));
  if (consume(token, "-="))
    node = new_node(ND_ASSIGN, node,
                    new_node(ND_SUB, copy_node(node), assign(token)));
  if (consume(token, "*="))
    node = new_node(ND_ASSIGN, node,
                    new_node(ND_MUL, copy_node(node), assign(token)));
  if (consume(token, "/="))
    node = new_node(ND_ASSIGN, node,
                    new_node(ND_DIV, copy_node(node), assign(token)));
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
    Variable *node_type = type(token, false, false);
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
    return new_node(ND_ASSIGN, node,
                    new_node(ND_ADD, copy_node(node), new_node_num(1)));
  }
  if (consume(token, "--")) {
    Node *node = primary(token);
    return new_node(ND_ASSIGN, node,
                    new_node(ND_SUB, copy_node(node), new_node_num(1)));
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
    Variable *cast_type = type(token, false, false);
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
      char *name = calloc(tok->len + 1, sizeof(char));
      memcpy(name, tok->str, tok->len);
      char *call_name = function_conversion(name);
      node->kind = ND_CALL;
      node->call = calloc(1, sizeof(Call));
      node->call->name = call_name;
      node->call->len = strlen(call_name);
      Function *func = get_function(tok);
      if (func != NULL) {
        node->call->ret = copy_var(func->ret);
      } else {
        if (strcmp(call_name, "llvm.va_start") == 0 ||
            strcmp(call_name, "llvm.va_end") == 0 ||
            strcmp(call_name, "memcpy") == 0 ||
            strcmp(call_name, "free") == 0 || strcmp(call_name, "exit") == 0) {
          node->call->ret = new_variable(-1, TYPE_VOID, NULL, 0);
        } else if (strcmp(call_name, "__errno_location") == 0) {
          node->call->ret = new_variable(
              -1, TYPE_PTR, new_variable(-1, TYPE_I32, NULL, 0), 0);
        } else {
          node->call->ret = new_variable(-1, TYPE_I32, NULL, 0);
        }
      }
      node->call->args = new_vector();
      while (!consume(token, ")")) {
        vec_push_last(node->call->args, ternary(token));
        if (consume(token, ")")) break;
        expect(token, ",");
      }
      if (strcmp(call_name, "llvm.va_start") == 0) {
        free_node(vec_pop(node->call->args));
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

  tok = copy_token(*token);
  if (consume_reserved(token, TK_STRING)) {
    Token *next_tok = *token;
    while (consume_reserved(token, TK_STRING)) {
      Token *new_tok = calloc(1, sizeof(Token));
      new_tok->str = calloc(next_tok->len + tok->len + 1, sizeof(char));
      new_tok->len = sprintf(new_tok->str, "%.*s%.*s", tok->len, tok->str,
                             next_tok->len, next_tok->str);
      free(tok->str);
      free(tok);
      tok = new_tok;
      next_tok = *token;
    }
    print_debug("string: %.*s", tok->len, tok->str);
    // エスケープ処理
    char *str = calloc(1024, sizeof(char));
    int str_index = 0;
    int len = 0;
    for (int i = 0; i < tok->len; i++) {
      if (tok->str[i] == '\\') {
        i++;
        if (tok->str[i] == 'a') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = '7';
        } else if (tok->str[i] == 'b') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = '8';
        } else if (tok->str[i] == 'f') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = 'C';

        } else if (tok->str[i] == 'e') {
          str[str_index++] = '\\';
          str[str_index++] = '1';
          str[str_index++] = 'B';
        } else if (tok->str[i] == 'f') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = 'C';
        } else if (tok->str[i] == 'n') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = 'A';
        } else if (tok->str[i] == 'r') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = 'D';
        } else if (tok->str[i] == 't') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = '9';
        } else if (tok->str[i] == 'v') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = 'B';
        } else if (tok->str[i] == '0') {
          str[str_index++] = '\\';
          str[str_index++] = '0';
          str[str_index++] = '0';
        } else if (tok->str[i] == '\"') {
          str[str_index++] = '\\';
          str[str_index++] = '2';
          str[str_index++] = '2';
        } else if (tok->str[i] == '\\') {
          str[str_index++] = '\\';
          str[str_index++] = '\\';
        } else {
          str[str_index++] = tok->str[i];
        }
        len++;
        continue;
      }
      str[str_index++] = tok->str[i];
      len++;
      if (str_index >= 1024) {
        error_at(tok->str, "文字列が長すぎます");
      }
    }
    print_debug("string: %.*s (len = %d)", tok->len, tok->str, tok->len);
    Token *escaped_tok = calloc(1, sizeof(Token));
    escaped_tok->str = str;
    escaped_tok->len = str_index;
    escaped_tok->val = len;
    print_debug("escaped string: %.*s (len = %d)", escaped_tok->len,
                escaped_tok->str, escaped_tok->len);

    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_STRING;
    node->offset = global_strings->size;
    vec_push_last(global_strings, escaped_tok);
    free(tok->str);
    free(tok);
    return node;
  }

  Token *char_tok = *token;
  if (consume_reserved(token, TK_CHARL)) {
    Node *node = new_node_num(*char_tok->str);
    node->cast = new_variable(-1, TYPE_I8, NULL, 0);
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

Variable *parse_struct(Token **token, bool is_declare, bool is_typedef) {
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
    tok->str = calloc(16, sizeof(char));
    sprintf(tok->str, "anon.%d", anon_structs_index++);
    tok->len = strlen(tok->str);
  }
  Variable *var = find_struct(tok);

  if (consume(token, "{")) {
    bool f_override = true;
    if (var != NULL) {
      if (var->reg != -1) {
        error_at(tok->str, "変数が二重定義されています");
        return NULL;
      }
    } else {
      var = calloc(1, sizeof(Variable));
      f_override = false;
    }
    var->type = TYPE_STRUCT;
    var->fields = new_vector();
    var->name = calloc(tok->len + 1, sizeof(char));
    memcpy(var->name, tok->str, tok->len);
    var->len = tok->len;
    var->reg = 0;
    if (!is_declare && !f_override) {
      vec_push_last(global_local_structs, var);
    }

    while (!consume(token, "}")) {
      Variable *field_type = type(token, false, false);
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
      field->name = calloc(tok->len + 1, sizeof(char));
      memcpy(field->name, tok->str, tok->len);
      field->len = tok->len;
      field->var = field_type;
      vec_push_last(var->fields, field);
      expect(token, ";");
    }
  } else {
    if (var == NULL) {
      if (!is_typedef) {
        error_at(tok->str, "未定義の構造体です: %.*s", tok->len, tok->str);
        return NULL;
      }

      var = new_variable(-1, TYPE_STRUCT, NULL, 0);
      var->name = calloc(tok->len + 1, sizeof(char));
      memcpy(var->name, tok->str, tok->len);
      var->len = tok->len;
      var->reg = -1;
      vec_push_last(global_structs, var);
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
      lvar->name = calloc(tok->len + 1, sizeof(char));
      memcpy(lvar->name, tok->str, tok->len);
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

Variable *type(Token **token, bool exclude_ptr, bool is_typedef) {
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
    if (var != NULL) {
      var = copy_var_if_needed(var);
      consume_ident(token);
    }
  }

  if (var == NULL) {
    var = parse_struct(token, false, is_typedef);
  }

  if (!exclude_ptr && var != NULL) {
    int ref_nest = 0;
    while (consume(token, "*")) ref_nest++;
    for (int nest = ref_nest; nest > 0; nest--) {
      Variable *ptr = new_variable(-1, TYPE_PTR, var, 0);
      var = ptr;
    }
  }

  return var;
}

Node *constant(Token **token) {
  print_debug_token("constant", token);

  Token *tok = consume_ident(token);
  if (tok != NULL) {
    LVar *lvar = find_lvar_from_vector(tok, global_globals);
    if (lvar == NULL) {
      error_at(tok->str, "未定義の変数です");
    }
    if (lvar->var->value == NULL) {
      error_at(tok->str, "定数ではありません");
    }
    return new_node_num(*lvar->var->value);
  }
  return new_node_num(expect_number(token));
}

Program *parse(Token *token) { return program(&token); }
