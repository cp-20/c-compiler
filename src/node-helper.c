#include "node-helper.h"

#include <stdlib.h>

#include "debug.h"
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
    case ND_LVAR: {
      int offset = node->offset;
      if (offset < 0) {
        // グローバル変数
        LVar* lvar = vec_at(global_globals, -offset - 1);
        return lvar->var;
      }

      for (int i = locals->size - 1; i >= 0; i--) {
        vector* local = vec_at(locals, i);
        for (int j = 0; j < local->size; j++) {
          if (offset == 0) {
            LVar* lvar = vec_at(local, j);
            return lvar->var;
          }
          offset--;
        }
      }
      error("ローカル変数が見つかりません (offset = %d)", node->offset);
    }
    case ND_NUM:
      return new_variable(0, TYPE_I32, NULL, 0);
    case ND_ACCESS: {
      Variable* st = get_node_type(node->lhs, locals);
      if (st->type != TYPE_STRUCT) {
        error("%.*s は構造体ではありません", st->len, st->name);
      }
      LVar* lvar = vec_at(st->fields, node->rhs->val);
      return lvar->var;
    }
    case ND_STRING:
      return new_variable(0, TYPE_PTR, new_variable(0, TYPE_I8, NULL, 0), 0);
    default:
      error("ノード %s の型はわかりません", get_node_kind_name(node->kind));
      return NULL;
  }
}
