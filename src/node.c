#include "node.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "code.h"
#include "debug.h"
#include "error.h"
#include "variable.h"

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

Code *print_node(Node *node) {
  Code *code = init_code();
  if (node == NULL) return code;

  if (node->cast != NULL) {
    char *cast_type = get_variable_type_str(node->cast);
    push_code(code, "(%s)", cast_type);
    free(cast_type);
  }

  if (node->kind == ND_NUM) {
    push_code(code, "%d", node->val);
    return code;
  }

  if (node->kind == ND_LVAR) {
    if (node->offset < 0) {
      push_code(code, "global%d", -node->offset - 1);
    } else {
      push_code(code, "var%d", node->offset);
    }
    if (node->kind == ND_ASSIGN) {
      merge_code(code, print_node(node->lhs));
      push_code(code, "=");
      merge_code(code, print_node(node->rhs));
    }
    return code;
  }

  if (node->kind == ND_RETURN) {
    push_code(code, "return ");
    merge_code(code, print_node(node->lhs));
    return code;
  }

  if (node->kind == ND_IF) {
    push_code(code, "if (");
    merge_code(code, print_node(node->lhs));
    push_code(code, ") ");
    merge_code(code, print_node(node->rhs));
    if (node->extra) {
      push_code(code, " else ");
      merge_code(code, print_node(node->extra));
    }
    return code;
  }

  if (node->kind == ND_WHILE) {
    push_code(code, "while (");
    merge_code(code, print_node(node->lhs));
    push_code(code, ") ");
    merge_code(code, print_node(node->rhs));
    return code;
  }

  if (node->kind == ND_DO) {
    push_code(code, "do ");
    merge_code(code, print_node(node->lhs));
    push_code(code, " while (");
    merge_code(code, print_node(node->rhs));
    push_code(code, ")");
    return code;
  }

  if (node->kind == ND_FOR) {
    push_code(code, "for (");
    if (node->lhs) print_node(node->lhs);
    push_code(code, ";");
    if (node->rhs) print_node(node->rhs);
    push_code(code, ";");
    if (node->extra) print_node(node->extra);
    push_code(code, ")");
    return code;
  }

  if (node->kind == ND_BLOCK) {
    push_code(code, "{ ");
    for (int i = 0; i < node->stmts->size; i++) {
      merge_code(code, print_node(vec_at(node->stmts, i)));
      push_code(code, "; ");
    }
    push_code(code, "}");
    return code;
  }

  if (node->kind == ND_GROUP) {
    for (int i = 0; i < node->stmts->size; i++) {
      merge_code(code, print_node(vec_at(node->stmts, i)));
      push_code(code, "; ");
    }
    return code;
  }

  if (node->kind == ND_CALL) {
    push_code(code, "%.*s(", node->call->len, node->call->name);
    for (int i = 0; i < node->call->args->size; i++) {
      if (i > 0) push_code(code, ", ");
      merge_code(code, print_node(vec_at(node->call->args, i)));
    }
    push_code(code, ")");
    return code;
  }

  if (node->kind == ND_INCR) {
    merge_code(code, print_node(node->lhs));
    push_code(code, "++");
    return code;
  }

  if (node->kind == ND_DECR) {
    merge_code(code, print_node(node->lhs));
    push_code(code, "--");
    return code;
  }

  if (node->kind == ND_REF) {
    push_code(code, "&");
    merge_code(code, print_node(node->lhs));
    return code;
  }

  if (node->kind == ND_DEREF) {
    push_code(code, "*");
    merge_code(code, print_node(node->lhs));
    return code;
  }

  if (node->kind == ND_ACCESS) {
    merge_code(code, print_node(node->lhs));
    push_code(code, ".");
    merge_code(code, print_node(node->rhs));
    return code;
  }

  if (node->kind == ND_TERNARY) {
    merge_code(code, print_node(node->lhs));
    push_code(code, " ? ");
    merge_code(code, print_node(node->rhs->lhs));
    push_code(code, " : ");
    merge_code(code, print_node(node->rhs->rhs));
    return code;
  }

  if (node->kind == ND_CONTINUE) {
    push_code(code, "continue;");
    return code;
  }

  if (node->kind == ND_BREAK) {
    push_code(code, "break;");
    return code;
  }

  if (node->kind == ND_SWITCH) {
    push_code(code, "switch (");
    merge_code(code, print_node(node->lhs));
    push_code(code, ") {");
    for (int i = 0; i < node->stmts->size; i++) {
      merge_code(code, print_node(vec_at(node->stmts, i)));
    }
    push_code(code, "}");
    return code;
  }

  if (node->kind == ND_CASE) {
    push_code(code, "case ");
    merge_code(code, print_node(node->lhs));
    push_code(code, ": ");
    merge_code(code, print_node(node->rhs));
    return code;
  }

  if (node->kind == ND_DEFAULT) {
    push_code(code, "default: ");
    merge_code(code, print_node(node->rhs));
    return code;
  }

  push_code(code, "(");
  merge_code(code, print_node(node->lhs));
  if (node->kind == ND_ADD) {
    push_code(code, "+");
  } else if (node->kind == ND_SUB) {
    push_code(code, "-");
  } else if (node->kind == ND_MUL) {
    push_code(code, "*");
  } else if (node->kind == ND_DIV) {
    push_code(code, "/");
  } else if (node->kind == ND_EQ) {
    push_code(code, "==");
  } else if (node->kind == ND_NE) {
    push_code(code, "!=");
  } else if (node->kind == ND_LT) {
    push_code(code, "<");
  } else if (node->kind == ND_LE) {
    push_code(code, "<=");
  } else if (node->kind == ND_GT) {
    push_code(code, ">");
  } else if (node->kind == ND_GE) {
    push_code(code, ">=");
  } else if (node->kind == ND_ASSIGN) {
    push_code(code, "=");
  } else if (node->kind == ND_AND) {
    push_code(code, "&&");
  } else if (node->kind == ND_OR) {
    push_code(code, "||");
  }
  merge_code(code, print_node(node->rhs));
  push_code(code, ")");

  return code;
}

char *get_node_kind_name(NodeKind kind) {
  switch (kind) {
    case ND_ADD:
      return "ND_ADD";
    case ND_SUB:
      return "ND_SUB";
    case ND_MUL:
      return "ND_MUL";
    case ND_DIV:
      return "ND_DIV";
    case ND_EQ:
      return "ND_EQ";
    case ND_NE:
      return "ND_NE";
    case ND_LT:
      return "ND_LT";
    case ND_LE:
      return "ND_LE";
    case ND_GT:
      return "ND_GT";
    case ND_GE:
      return "ND_GE";
    case ND_AND:
      return "ND_AND";
    case ND_OR:
      return "ND_OR";
    case ND_INCR:
      return "ND_INCR";
    case ND_DECR:
      return "ND_DECR";
    case ND_REF:
      return "ND_REF";
    case ND_DEREF:
      return "ND_DEREF";
    case ND_ACCESS:
      return "ND_ACCESS";
    case ND_ASSIGN:
      return "ND_ASSIGN";
    case ND_LVAR:
      return "ND_LVAR";
    case ND_RETURN:
      return "ND_RETURN";
    case ND_IF:
      return "ND_IF";
    case ND_WHILE:
      return "ND_WHILE";
    case ND_DO:
      return "ND_DO";
    case ND_FOR:
      return "ND_FOR";
    case ND_CONTINUE:
      return "ND_CONTINUE";
    case ND_BREAK:
      return "ND_BREAK";
    case ND_SWITCH:
      return "ND_SWITCH";
    case ND_CASE:
      return "ND_CASE";
    case ND_DEFAULT:
      return "ND_DEFAULT";
    case ND_BLOCK:
      return "ND_BLOCK";
    case ND_GROUP:
      return "ND_GROUP";
    case ND_CALL:
      return "ND_CALL";
    case ND_NUM:
      return "ND_NUM";
    case ND_STRING:
      return "ND_STRING";
    case ND_TERNARY:
      return "ND_TERNARY";
    default:
      return "UNKNOWN";
  }
}

void free_call(Call *call) {
  if (call == NULL) return;
  free(call->name);
  free_variable(call->ret);
  for (int i = 0; i < call->args->size; i++) {
    free_node(vec_at(call->args, i));
  }
  vec_free(call->args);
  free(call);
}

void free_node(Node *node) {
  if (node == NULL) return;
  free_node(node->lhs);
  free_node(node->rhs);
  free_node(node->extra);
  free_node(node->extra2);
  free_call(node->call);
  if (node->stmts != NULL) {
    for (int i = 0; i < node->stmts->size; i++) {
      free_node(vec_at(node->stmts, i));
    }
    vec_free(node->stmts);
  }
  free(node);
}

void free_function(Function *func) {
  if (func == NULL) return;
  free(func->name);
  free_variable(func->ret);
  for (int i = 0; i < func->locals->size; i++) {
    free_lvar(vec_at(func->locals, i));
  }
  vec_free(func->locals);
  for (int i = 0; i < func->structs->size; i++) {
    free_struct(vec_at(func->structs, i));
  }
  vec_free(func->structs);
  for (int i = 0; i < func->body->size; i++) {
    free_node(vec_at(func->body, i));
  }
  vec_free(func->body);
  free(func);
}

Call *copy_call(Call *call) {
  if (call == NULL) return NULL;
  Call *new_call = calloc(1, sizeof(Call));
  new_call->name = calloc(call->len + 1, sizeof(char));
  memcpy(new_call->name, call->name, call->len);
  new_call->len = call->len;
  new_call->ret = copy_var(call->ret);
  if (call->args != NULL) {
    new_call->args = new_vector();
    for (int i = 0; i < call->args->size; i++) {
      vec_push_last(new_call->args, copy_node(vec_at(call->args, i)));
    }
  }
  return new_call;
}

Node *copy_node(Node *node) {
  if (node == NULL) return NULL;
  Node *new_node = calloc(1, sizeof(Node));
  new_node->kind = node->kind;
  new_node->lhs = copy_node(node->lhs);
  new_node->rhs = copy_node(node->rhs);
  new_node->extra = copy_node(node->extra);
  new_node->extra2 = copy_node(node->extra2);
  new_node->call = copy_call(node->call);
  if (node->stmts != NULL) {
    new_node->stmts = new_vector();
    for (int i = 0; i < node->stmts->size; i++) {
      vec_push_last(new_node->stmts, copy_node(vec_at(node->stmts, i)));
    }
  }
  if (node->locals != NULL) {
    new_node->locals = new_vector();
    for (int i = 0; i < node->locals->size; i++) {
      vec_push_last(new_node->locals, copy_lvar(vec_at(node->locals, i)));
    }
  }
  new_node->cast = copy_var(node->cast);
  new_node->val = node->val;
  new_node->offset = node->offset;
  return new_node;
}
