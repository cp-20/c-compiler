#include "node.h"

#include <stdio.h>
#include <stdlib.h>

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

  if (node->kind == ND_GROUP) {
    for (int i = 0; i < node->stmts->size; i++) {
      print_node(vec_at(node->stmts, i));
      printf("; ");
    }
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
