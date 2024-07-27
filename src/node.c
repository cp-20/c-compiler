#include "node.h"

#include <stdio.h>
#include <stdlib.h>

#include "code.h"
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
    push_code(code, "(%s)", get_variable_type_str(node->cast));
  }

  if (node->kind == ND_NUM) {
    push_code(code, "%d", node->val);
    return code;
  }

  if (node->kind == ND_LVAR) {
    int offset = node->offset;
    if (node->offset < 0) {
      offset = -node->offset - 1 - 32;
    }
    push_code(code, "%c", 'a' + offset);
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
