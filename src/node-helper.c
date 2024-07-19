#include "node-helper.h"

#include <stdlib.h>

#include "error.h"
#include "node.h"
#include "parser.h"
#include "variable.h"
#include "vector.h"

Variable* get_node_type(Node* node, vector* locals) {
  if (node->cast != NULL) {
    return node->cast;
  }

  switch (node->kind) {
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
      return get_calc_result_type(node->kind, get_node_type(node->lhs, locals),
                                  get_node_type(node->rhs, locals));
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
    case ND_GT:
    case ND_GE:
    case ND_AND:
    case ND_OR:
      return new_variable(0, TYPE_I32, NULL, 0);
    case ND_INCR:
    case ND_DECR:
      return get_node_type(node->lhs, locals);
    case ND_REF:
      return new_variable(0, TYPE_PTR, get_node_type(node->lhs, locals), 0);
    case ND_DEREF:
      return get_node_type(node->lhs, locals)->ptr_to;
    case ND_ASSIGN:
      return get_node_type(node->lhs, locals);
    case ND_LVAR:
      return ((LVar*)vec_at(locals, node->offset))->var;
    case ND_NUM:
      return new_variable(0, TYPE_I32, NULL, 0);
    default:
      error("ノード %d はsizeof演算ができません", node->kind);
      return NULL;
  }
}
