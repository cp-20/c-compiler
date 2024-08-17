#include "generator.h"

#include <stdio.h>

#include "code.h"
#include "color.h"
#include "debug.h"
#include "error.h"
#include "llvm.h"
#include "parser.h"
#include "seek.h"
#include "variable.h"
#include "vector.h"

Variable** globals;

vector* continue_labels;
vector* break_labels;
Variable* return_type;

Variable* get_variable(Variable** locals, int offset) {
  Variable* var = offset >= 0 ? locals[offset] : globals[-offset - 1];
  return copy_var_if_needed(var);
}

// 左辺値のポインタ (レジスタ) を返す
Variable* gen_lval(Code* code, Node* node, vector* stack, Variable** locals_r,
                   int* rctx) {
  if (node->kind == ND_LVAR) {
    return get_variable(locals_r, node->offset);
  }

  if (node->kind == ND_DEREF) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx, true));
    Variable* lvar_val = pop_variable(stack);
    if (!is_pointer_like(lvar_val)) {
      error("間接参照演算子の右辺値がポインタではありません");
      return NULL;
    }
    char* rhs_type = get_variable_type_str(lvar_val);
    char* rhs_ptype = get_ptr_variable_type_str(lvar_val);
    int size = get_variable_size(lvar_val);
    int r_ptr_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_ptr_val,
              rhs_type, rhs_ptype, lvar_val->reg, size);
    Variable* lvar = with_reg(lvar_val->ptr_to, r_ptr_val);
    free_variable(lvar_val);
    return lvar;
  }

  if (node->kind == ND_ACCESS) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx, true));
    Variable* lvar_val = pop_variable(stack);
    char* type = get_variable_type_str(lvar_val);
    char* ptype = get_ptr_variable_type_str(lvar_val);
    int r_field = r_register(rctx);
    push_code(code,
              "  %%%d = getelementptr inbounds %s, %s %%%d, i32 0, i32 %d\n",
              r_field, type, ptype, lvar_val->reg, node->rhs->val);
    LVar* field = vec_at(lvar_val->fields, node->rhs->val);
    Variable* field_var = with_reg(field->var, r_field);
    free_variable(lvar_val);
    free(type);
    free(ptype);
    return field_var;
  }

  error("代入の左辺値が変数ではありません (kind = %s)",
        get_node_kind_name(node->kind));
  return NULL;
}

int eval_cond(Code* code, Node* node, vector* stack, Variable** locals_r,
              int* rctx) {
  bool is_boolean = node->kind == ND_EQ || node->kind == ND_NE ||
                    node->kind == ND_LT || node->kind == ND_LE ||
                    node->kind == ND_GT || node->kind == ND_GE ||
                    node->kind == ND_AND || node->kind == ND_OR;
  if (is_boolean) {
    node->cast = new_variable(-10, TYPE_I1, NULL, 0);
  }
  merge_code(code, generate_node(node, stack, locals_r, rctx, false));

  Variable* cond = pop_variable(stack);
  char* cond_type = get_variable_type_str(cond);
  int r_cond;
  if (is_boolean) {
    r_cond = cond->reg;
  } else {
    char* cond_val = get_variable_value_str(cond, code, rctx, false);
    r_cond = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %s, %s\n", r_cond, cond_type, cond_val,
              cond->type == TYPE_PTR ? "null" : "0");
    free(cond_val);
  }

  free_variable(cond);
  free(cond_type);

  return r_cond;
}

void push_variable_with_cast_if_needed(vector* stack, Variable* var,
                                       Variable* cast) {
  if (cast != NULL) {
    Variable* new_var = with_reg(cast, var->reg);
    if (var->value != NULL) {
      new_var->value = malloc(sizeof(int));
      *new_var->value = *var->value;
    }
    push_variable(stack, new_var);
    // 怪しい
    free_variable(var);
  } else {
    push_variable(stack, var);
  }
}

Code* generate_node(Node* node, vector* stack, Variable** locals_r, int* rctx,
                    bool is_lval) {
  print_debug(COL_BLUE "[generator]" COL_CYAN " [generate_node]" COL_RESET
                       " %s",
              get_node_kind_name(node->kind));

  Code* code = init_code();
  push_code(code, "  ; generate %d -> ", node->kind);
  merge_code(code, print_node(node));
  push_code(code, "\n");

  // ブロックスコープを作る
  vector* pop_local_list = new_vector();
  if (node->kind == ND_BLOCK || node->kind == ND_FOR) {
    print_debug(COL_BLUE "[generator] " COL_GREEN "[%s] " COL_RESET
                         "node->locals->size = %d",
                node->kind == ND_BLOCK ? "ND_BLOCK" : "ND_FOR",
                node->locals->size);
    for (int i = 0; i < node->locals->size; i++) {
      LVar* lvar = vec_at(node->locals, i);
      char* type = get_variable_type_str(lvar->var);
      int size = get_variable_size(lvar->var);
      int r_var = r_register(rctx);
      push_code(code, "  %%%d = alloca %s, align %d\n", r_var, type, size);
      vec_push_last(pop_local_list, locals_r[lvar->offset]);
      locals_r[lvar->offset] = with_reg(lvar->var, r_var);
      free(type);
    }
  }

  switch (node->kind) {
    case ND_NUM: {
      bool is_null = node->cast != NULL && node->val == 0;
      if (is_null) is_null = node->cast->type == TYPE_PTR;
      if (is_null) is_null = node->cast->ptr_to->type == TYPE_VOID;

      Variable* var = new_variable(is_null ? -3 : -2, TYPE_I32, NULL, 0);
      var->value = malloc(sizeof(int));
      *var->value = node->val;
      push_variable_with_cast_if_needed(stack, var, node->cast);
      break;
    }
    case ND_STRING: {
      Variable* var =
          new_variable(-4, TYPE_PTR, new_variable(-1, TYPE_I8, NULL, 0), 0);
      var->value = malloc(sizeof(int));
      *var->value = node->offset;
      push_variable_with_cast_if_needed(stack, var, node->cast);
      break;
    }
    case ND_RETURN: {
      if (node->lhs == NULL) {
        push_code(code, "  ret void\n");
        r_register(rctx);
        break;
      }
      merge_code(code,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      Variable* val = pop_variable(stack);
      char* val_type = get_variable_type_str(val);
      char* result_val = get_variable_value_str(val, code, rctx, is_lval);
      if (return_type->type == TYPE_I32 && val->type == TYPE_I8) {
        int r_right_i32_val = r_register(rctx);
        push_code(code, "  %%%d = zext i8 %s to i32\n", r_right_i32_val,
                  result_val);
        free(result_val);
        result_val = get_variable_value_str(with_reg(val, r_right_i32_val),
                                            code, rctx, is_lval);
        free(val_type);
        val_type = calloc(4, sizeof(char));
        sprintf(val_type, "i32");
      }
      push_code(code, "  ret %s %s\n", val_type, result_val);
      // なぜかよくわからないけどレジスタを1個空けると上手く行く
      r_register(rctx);
      free_variable(val);
      free(val_type);
      break;
    }
    case ND_LVAR: {
      Variable* var = get_variable(locals_r, node->offset);
      if (var->value != NULL) {
        // enum の場合は数値に置き換える
        merge_code(code, generate_node(new_node_num(*var->value), stack,
                                       locals_r, rctx, is_lval));
      } else if (var->type == TYPE_ARRAY) {
        // 配列の場合はポインタを返す
        int r_var = r_register(rctx);
        char* var_type = get_variable_type_str(var);
        push_code(
            code,
            "  %%%d = getelementptr inbounds %s, ptr %%%d, i64 0, i64 0\n",
            r_var, var_type, var->reg);
        Variable* var_ptr = new_variable(r_var, TYPE_PTR, var->ptr_to, 0);
        push_variable_with_cast_if_needed(stack, var_ptr, node->cast);
        free(var_type);
      } else {
        Variable* var_copy = copy_var(var);
        var_copy->value = malloc(sizeof(int));
        *var_copy->value = -1;
        push_variable_with_cast_if_needed(stack, var_copy, node->cast);
      }

      break;
    }
    case ND_ASSIGN: {
      Variable* lvar = gen_lval(code, node->lhs, stack, locals_r, rctx);
      if (node->rhs->kind == ND_CALL) {
        free(node->rhs->call->ret);
        node->rhs->call->ret = copy_var_if_needed(lvar);
      }
      merge_code(code,
                 generate_node(node->rhs, stack, locals_r, rctx, is_lval));
      Variable* rhs = pop_variable(stack);
      print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_ASSIGN] " COL_RESET
                           "rhs->value = %p",
                  rhs->value);
      if (lvar->type == TYPE_I32 && rhs->type == TYPE_I8) {
        int r_right_i32_val = r_register(rctx);
        push_code(code, "  %%%d = zext i8 %%%d to i32\n", r_right_i32_val,
                  rhs->reg);
        free_variable(rhs);
        rhs = new_variable(r_right_i32_val, TYPE_I32, NULL, 0);
      }

      if (!is_same_type(lvar, rhs)) {
        error("代入の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s",
              get_variable_type_str(lvar), get_variable_type_str(rhs));
      }
      char* type = get_variable_type_str(rhs);
      char* ptype = get_ptr_variable_type_str(rhs);
      int size = get_variable_size(rhs);
      char* rval = get_variable_value_str(rhs, code, rctx, is_lval);
      if (node->lhs->offset >= 0) {
        push_code(code, "  store %s %s, %s %%%d, align %d\n", type, rval, ptype,
                  lvar->reg, size);
      } else {
        push_code(code, "  store %s %s, %s @%.*s, align %d\n", type, rval,
                  ptype, lvar->len, lvar->name, size);
      }
      if (rhs->value != NULL) {
        lvar->value = malloc(sizeof(int));
        *lvar->value = *rhs->value;
      }
      push_variable_with_cast_if_needed(stack, with_reg(lvar, rhs->reg),
                                        node->cast);
      free_variable(rhs);
      free(type);
      free(ptype);
      free(rval);
      break;
    }
    case ND_IF: {
      int r_cond_bool = eval_cond(code, node->lhs, stack, locals_r, rctx);

      int r_then_label = r_register(rctx);

      Code* block = init_code();
      push_code(block, "%d:\n", r_then_label);
      merge_code(block,
                 generate_node(node->rhs, stack, locals_r, rctx, is_lval));

      int r_next_label = r_register(rctx);

      if (node->extra != NULL) {
        Code* sub_block = init_code();
        push_code(sub_block, "%d:\n", r_next_label);
        merge_code(sub_block,
                   generate_node(node->extra, stack, locals_r, rctx, is_lval));
        int r_final_label = r_register(rctx);
        push_code(sub_block, "  br label %%%d\n", r_final_label);
        push_code(block, "  br label %%%d\n", r_final_label);

        merge_code(block, sub_block);
        push_code(block, "%d:\n", r_final_label);
      } else {
        push_code(block, "  br label %%%d\n", r_next_label);
        push_code(block, "%d:\n", r_next_label);
      }

      push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
                r_then_label, r_next_label);
      merge_code(code, block);
      break;
    }
    case ND_WHILE: {
      // start_label:
      //   条件式の評価
      //   br i1 %cond, label %next_label, label %end_label
      // break_label:
      //   br label %end_label
      // next_label:
      //   ループ内の処理
      // end_label:

      int r_start_label = r_register(rctx);
      int* r_continue_label_ptr = malloc(sizeof(int));
      *r_continue_label_ptr = r_start_label;
      vec_push_last(continue_labels, r_continue_label_ptr);
      push_code(code, "  br label %%%d\n", r_start_label);
      push_code(code, "%d:\n", r_start_label);

      // 条件式を計算する
      int r_cond_bool = eval_cond(code, node->lhs, stack, locals_r, rctx);

      Code* break_block = init_code();
      int r_break_label = r_register(rctx);
      int* r_break_label_ptr = malloc(sizeof(int));
      *r_break_label_ptr = r_break_label;
      vec_push_last(break_labels, r_break_label_ptr);
      push_code(break_block, "%d:\n", r_break_label);

      Code* block = init_code();

      int r_next_label = r_register(rctx);
      push_code(block, "%d:\n", r_next_label);
      merge_code(block,
                 generate_node(node->rhs, stack, locals_r, rctx, is_lval));
      push_code(block, "  br label %%%d\n", r_start_label);

      int r_end_label = r_register(rctx);
      push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
                r_next_label, r_end_label);
      push_code(break_block, "  br label %%%d\n", r_end_label);
      merge_code(code, break_block);
      merge_code(code, block);
      push_code(code, "%d:\n", r_end_label);
      vec_pop(continue_labels);
      vec_pop(break_labels);
      free(r_continue_label_ptr);
      free(r_break_label_ptr);
      break;
    }
    case ND_DO: {
      //   br label %start_label
      // break_label:
      //   br label %end_label
      // start_label(continue_label):
      //   ループ内の処理
      //   条件式の評価
      //   br i1 %cond, label %start_label, label %end_label
      // end_label:

      Code* break_block = init_code();
      int r_break_label = r_register(rctx);
      int* r_break_label_ptr = malloc(sizeof(int));
      *r_break_label_ptr = r_break_label;
      vec_push_last(break_labels, r_break_label_ptr);
      push_code(break_block, "%d:\n", r_break_label);

      int r_start_label = r_register(rctx);
      int* r_continue_label_ptr = malloc(sizeof(int));
      *r_continue_label_ptr = r_start_label;
      vec_push_last(continue_labels, r_continue_label_ptr);
      push_code(code, "  br label %%%d\n", r_start_label);

      Code* block = init_code();
      push_code(block, "%d:\n", r_start_label);
      merge_code(block,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      int r_cond_bool = eval_cond(block, node->rhs, stack, locals_r, rctx);
      int r_end_label = r_register(rctx);
      push_code(block, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
                r_start_label, r_end_label);
      push_code(break_block, "  br label %%%d\n", r_end_label);
      merge_code(code, break_block);
      push_code(block, "%d:\n", r_end_label);
      merge_code(code, block);

      vec_pop(continue_labels);
      vec_pop(break_labels);
      free(r_continue_label_ptr);
      free(r_break_label_ptr);
      break;
    }
    case ND_FOR: {
      //   初期化式
      // start_label:
      //   条件式の評価
      //   br i1 %cond, label %next_label, label %end_label
      // continue_label:
      //   br label %continue_dist_label
      // break_label:
      //   br label %end_label
      // next_label:
      //   ループ内の処理
      //   br label %continue_dist_label
      // continue_dist_label:
      //   ループ後の処理
      //   br label %start_label
      // end_label:

      // 初期化式
      if (node->lhs != NULL) {
        merge_code(code,
                   generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      }

      int r_start_label = r_register(rctx);
      push_code(code, "  br label %%%d\n", r_start_label);
      push_code(code, "%d:\n", r_start_label);

      // 条件式を計算する
      int r_cond_bool;
      if (node->rhs != NULL) {
        r_cond_bool = eval_cond(code, node->rhs, stack, locals_r, rctx);
      } else {
        r_cond_bool = r_register(rctx);
        push_code(code, "  %%%d = icmp ne i32 1, 0\n", r_cond_bool);
      }

      Code* break_block = init_code();
      int r_break_label = r_register(rctx);
      int* r_break_label_ptr = malloc(sizeof(int));
      *r_break_label_ptr = r_break_label;
      vec_push_last(break_labels, r_break_label_ptr);
      push_code(break_block, "%d:\n", r_break_label);

      Code* continue_block = init_code();
      int r_continue_label = r_register(rctx);
      int* r_continue_label_ptr = malloc(sizeof(int));
      *r_continue_label_ptr = r_continue_label;
      vec_push_last(continue_labels, r_continue_label_ptr);
      push_code(continue_block, "%d:\n", r_continue_label);

      Code* block = init_code();

      int r_next_label = r_register(rctx);
      push_code(block, "%d:\n", r_next_label);
      merge_code(block,
                 generate_node(node->extra2, stack, locals_r, rctx, is_lval));
      int r_continue_dist_label = r_register(rctx);
      push_code(continue_block, "  br label %%%d\n", r_continue_dist_label);
      push_code(block, "  br label %%%d\n", r_continue_dist_label);
      push_code(block, "%d:\n", r_continue_dist_label);
      if (node->extra != NULL) {
        merge_code(block,
                   generate_node(node->extra, stack, locals_r, rctx, is_lval));
      }
      push_code(block, "  br label %%%d\n", r_start_label);

      int r_end_label = r_register(rctx);
      push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
                r_next_label, r_end_label);
      push_code(break_block, "  br label %%%d\n", r_end_label);
      merge_code(code, break_block);
      merge_code(code, continue_block);
      merge_code(code, block);
      push_code(code, "%d:\n", r_end_label);
      vec_pop(continue_labels);
      vec_pop(break_labels);
      free(r_continue_label_ptr);
      free(r_break_label_ptr);
      break;
    }
    case ND_BLOCK:
    case ND_GROUP: {
      print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_BLOCK] " COL_RESET
                           "node->stmts->size = %d",
                  node->stmts->size);
      // ブロックの場合は各文を順に生成する
      for (int i = 0; i < node->stmts->size; i++) {
        Node* stmt = vec_at(node->stmts, i);
        merge_code(code, generate_node(stmt, stack, locals_r, rctx, is_lval));
      }
      break;
    }
    case ND_CALL: {
      // 引数を計算する
      print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_CALL] " COL_RESET
                           "node->call->args->size = %d",
                  node->call->args->size);
      Variable** args = calloc(node->call->args->size, sizeof(Variable));
      char** arg_vals = calloc(node->call->args->size, sizeof(char*));
      for (int i = 0; i < node->call->args->size; i++) {
        Node* arg = vec_at(node->call->args, i);
        print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_CALL] " COL_RESET
                             "node->call->args[%d] = %d",
                    i, arg->kind);
        merge_code(code, generate_node(arg, stack, locals_r, rctx, is_lval));
        args[i] = pop_variable(stack);
        arg_vals[i] = get_variable_value_str(args[i], code, rctx, is_lval);
      }

      // 関数呼び出し本体
      char* return_type = get_variable_type_str(node->call->ret);
      if (node->cast != NULL) {
        free(return_type);
        return_type = get_variable_type_str(node->cast);
      }
      char* return_ptype = get_ptr_variable_type_str(node->call->ret);
      if (node->cast != NULL) {
        free(return_ptype);
        return_ptype = get_ptr_variable_type_str(node->cast);
      }
      int r_result_val;
      if (node->call->ret->type == TYPE_VOID) {
        push_code(code, "  call void @%.*s(", node->call->len,
                  node->call->name);
      } else {
        r_result_val = r_register(rctx);
        push_code(code, "  %%%d = call %s @%.*s(", r_result_val, return_type,
                  node->call->len, node->call->name);
      }
      for (int i = 0; i < node->call->args->size; i++) {
        if (i > 0) push_code(code, ", ");
        char* arg_type = get_variable_type_str(args[i]);
        char* arg_val = arg_vals[i];
        push_code(code, "%s noundef %s", arg_type, arg_val);
      }
      push_code(code, ")\n");

      // 結果をスタックに積む
      if (node->call->ret->type != TYPE_VOID) {
        push_variable_with_cast_if_needed(
            stack, with_reg(node->call->ret, r_result_val), node->cast);
      }

      for (int i = 0; i < node->call->args->size; i++) {
        free_variable(args[i]);
      }
      free(args);
      free(return_type);
      free(return_ptype);
      break;
    }
    case ND_INCR:
    case ND_DECR: {
      merge_code(code,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      Variable* var = pop_variable(stack);
      free(var->value);
      var->value = NULL;
      char* var_type = get_variable_type_str(var);
      char* var_ptype = get_ptr_variable_type_str(var);
      int var_size = get_variable_size(var);
      int r_var_val = r_register(rctx);
      push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_var_val,
                var_type, var_ptype, var->reg, var_size);
      push_variable_with_cast_if_needed(stack, with_reg(var, r_var_val),
                                        node->cast);
      NodeKind op_type = node->kind == ND_INCR ? ND_ADD : ND_SUB;
      Node* op_node = new_node(ND_ASSIGN, node->lhs,
                               new_node(op_type, node->lhs, new_node_num(1)));
      merge_code(code, generate_node(op_node, stack, locals_r, rctx, is_lval));
      free_variable(pop_variable(stack));
      break;
    }
    case ND_REF: {
      Variable* lvar = gen_lval(code, node->lhs, stack, locals_r, rctx);
      Variable* lvar_type = new_variable(-5, TYPE_PTR, lvar, 0);
      lvar_type->reg = lvar->reg;
      if (lvar->value != NULL) {
        lvar_type->value = malloc(sizeof(int));
        *lvar_type->value = *lvar->value;
      }
      push_variable_with_cast_if_needed(stack, lvar_type, node->cast);
      break;
    }
    case ND_DEREF: {
      merge_code(code,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      Variable* val_ptr = pop_variable(stack);
      if (!is_pointer_like(val_ptr)) {
        error("間接参照演算子の右辺値がポインタではありません");
      }
      Variable* val = val_ptr->ptr_to;
      bool is_struct = val->type == TYPE_STRUCT;
      char* type = get_variable_type_str(is_struct ? val_ptr : val);
      char* ptype = get_ptr_variable_type_str(is_struct ? val_ptr : val);
      char* val_ptr_val =
          get_variable_value_str(val_ptr, code, rctx, is_struct || is_lval);
      int size = get_variable_size(is_struct ? val_ptr : val);
      int r_val = r_register(rctx);
      val = with_reg(val, r_val);
      push_code(code, "  %%%d = load %s, %s %s, align %d\n", r_val, type, ptype,
                val_ptr_val, size);
      push_variable_with_cast_if_needed(stack, val, node->cast);
      free(type);
      free(ptype);
      break;
    }
    case ND_ACCESS: {
      merge_code(code,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      Variable* lvar = pop_variable(stack);
      char* type = get_variable_type_str(lvar);
      char* ptype = get_ptr_variable_type_str(lvar);
      int r_field = r_register(rctx);
      push_code(code,
                "  %%%d = getelementptr inbounds %s, %s %%%d, i32 0, i32 %d\n",
                r_field, type, ptype, lvar->reg, node->rhs->val);
      LVar* field = vec_at(lvar->fields, node->rhs->val);
      Variable* field_var = with_reg(field->var, r_field);
      if (!is_lval) {
        int r_field_val = r_register(rctx);
        char* field_type = get_variable_type_str(field_var);
        char* field_ptype = get_ptr_variable_type_str(field_var);
        int field_size = get_variable_size(field_var);
        push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_field_val,
                  field_type, field_ptype, r_field, field_size);
        Variable* new_field_var = with_reg(field_var, r_field_val);
        free_variable(field_var);
        field_var = new_field_var;
        free(field_type);
        free(field_ptype);
      }
      push_variable_with_cast_if_needed(stack, field_var, node->cast);
      free_variable(lvar);
      free(type);
      free(ptype);
      break;
    }
    case ND_TERNARY: {
      int r_cond_bool = eval_cond(code, node->lhs, stack, locals_r, rctx);

      int r_result = r_register(rctx);
      char* result_type = get_variable_type_str(node->cast);
      char* result_ptype = get_ptr_variable_type_str(node->cast);
      int result_size = get_variable_size(node->cast);
      push_code(code, "  %%%d = alloca %s, align %d\n", r_result, result_type,
                result_size);
      int r_then_label = r_register(rctx);

      Code* block = init_code();
      push_code(block, "%d:\n", r_then_label);
      print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_TERNARY] " COL_RESET
                           "generate ternary 1");
      merge_code(block,
                 generate_node(node->rhs->lhs, stack, locals_r, rctx, is_lval));
      print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_TERNARY] " COL_RESET
                           "generate ternary 2");
      Variable* lhs_result = pop_variable(stack);
      char* lhs_type = get_variable_type_str(lhs_result);
      char* lhs_ptype = get_ptr_variable_type_str(lhs_result);
      char* lhs_val = get_variable_value_str(lhs_result, code, rctx, is_lval);
      int lhs_size = get_variable_size(lhs_result);
      push_code(block, "  store %s %s, %s %%%d, align %d\n", lhs_type, lhs_val,
                lhs_ptype, r_result, lhs_size);

      int r_next_label = r_register(rctx);

      Code* sub_block = init_code();
      push_code(sub_block, "%d:\n", r_next_label);
      merge_code(sub_block,
                 generate_node(node->rhs->rhs, stack, locals_r, rctx, is_lval));
      Variable* rhs_result = pop_variable(stack);
      char* rhs_type = get_variable_type_str(rhs_result);
      char* rhs_ptype = get_ptr_variable_type_str(rhs_result);
      char* rhs_val = get_variable_value_str(rhs_result, code, rctx, is_lval);
      int rhs_size = get_variable_size(rhs_result);
      push_code(sub_block, "  store %s %s, %s %%%d, align %d\n", rhs_type,
                rhs_val, rhs_ptype, r_result, rhs_size);
      int r_final_label = r_register(rctx);
      push_code(sub_block, "  br label %%%d\n", r_final_label);
      push_code(block, "  br label %%%d\n", r_final_label);

      merge_code(block, sub_block);
      push_code(block, "%d:\n", r_final_label);

      push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
                r_then_label, r_next_label);
      merge_code(code, block);
      int r_result_val = r_register(rctx);
      push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_result_val,
                result_type, result_ptype, r_result, result_size);
      push_variable(stack, with_reg(node->cast, r_result_val));
      free(result_type);
      free(result_ptype);
      free(lhs_type);
      free(lhs_ptype);
      free(rhs_type);
      free(rhs_ptype);
      break;
    }
    case ND_CONTINUE: {
      if (continue_labels->size == 0) {
        error("continue文がループの外で使われています");
      }
      int* r_continue_label_ptr = vec_last(continue_labels);
      push_code(code, "  br label %%%d\n", *r_continue_label_ptr);
      r_register(rctx);
      break;
    }
    case ND_BREAK: {
      if (break_labels->size == 0) {
        error("break文がループの外で使われています");
      }
      int* r_break_label_ptr = vec_last(break_labels);
      push_code(code, "  br label %%%d\n", *r_break_label_ptr);
      r_register(rctx);
      break;
    }
    case ND_SWITCH: {
      // switch文の条件となる値
      merge_code(code,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      Variable* value = pop_variable(stack);
      char* value_type = get_variable_type_str(value);
      char* value_val = get_variable_value_str(value, code, rctx, is_lval);

      // breakのラベル
      Code* break_block = init_code();
      int r_break_label = r_register(rctx);
      int* r_break_label_ptr = malloc(sizeof(int));
      *r_break_label_ptr = r_break_label;
      vec_push_last(break_labels, r_break_label_ptr);
      push_code(break_block, "%d:\n", r_break_label);

      // switchの各文
      Code* switch_cases = init_code();
      int* case_labels = calloc(node->stmts->size, sizeof(int));
      int r_next_case_label = r_register(rctx);
      push_code(switch_cases, "%d:\n", r_next_case_label);
      for (int i = 0; i < node->stmts->size; i++) {
        case_labels[i] = r_next_case_label;
        Node* case_node = vec_at(node->stmts, i);
        merge_code(switch_cases, generate_node(case_node->rhs, stack, locals_r,
                                               rctx, is_lval));
        r_next_case_label = r_register(rctx);
        push_code(switch_cases, "  br label %%%d\n", r_next_case_label);
        push_code(switch_cases, "%d:\n", r_next_case_label);
      }

      // defaultの文
      int default_label = r_next_case_label;
      if (node->rhs != NULL) {
        merge_code(switch_cases, generate_node(node->rhs->rhs, stack, locals_r,
                                               rctx, is_lval));
        r_next_case_label = r_register(rctx);
        push_code(switch_cases, "  br label %%%d\n", r_next_case_label);
        push_code(switch_cases, "%d:\n", r_next_case_label);
      }

      push_code(code, "  switch %s %s, label %%%d [\n", value_type, value_val,
                default_label);
      for (int i = 0; i < node->stmts->size; i++) {
        Node* case_node = vec_at(node->stmts, i);
        Node* case_value_node = case_node->lhs;
        int value = case_value_node->val;
        push_code(code, "    i32 %d, label %%%d\n", value, case_labels[i]);
      }
      push_code(code, "  ]\n");

      push_code(break_block, "  br label %%%d\n", r_next_case_label);
      merge_code(code, break_block);
      merge_code(code, switch_cases);

      free_variable(value);
      free(value_type);
      free(r_break_label_ptr);
      free(case_labels);
      break;
    }
    case ND_CASE:
    case ND_DEFAULT: {
      error("ND_CASE / ND_DEFAULT が生成されることはない");
      break;
    }
    case ND_ADD:
    case ND_SUB:
    case ND_MUL:
    case ND_DIV:
    case ND_EQ:
    case ND_NE:
    case ND_LT:
    case ND_LE:
    case ND_GE:
    case ND_GT:
    case ND_AND:
    case ND_OR: {
      bool is_boolean = node->cast != NULL;
      if (is_boolean) is_boolean = node->cast->type == TYPE_I1;

      // 演算子の場合は左右のノードを先に計算する
      merge_code(code,
                 generate_node(node->lhs, stack, locals_r, rctx, is_lval));
      merge_code(code,
                 generate_node(node->rhs, stack, locals_r, rctx, is_lval));

      // スタックから2つ取り出す
      Variable* rval = pop_variable(stack);
      Variable* lval = pop_variable(stack);

      if (lval->type == TYPE_I32 && rval->type == TYPE_I8) {
        int r_right_ext_val = r_register(rctx);
        push_code(code, "  %%%d = sext i8 %%%d to i32\n", r_right_ext_val,
                  rval->reg);
        rval = new_variable(r_right_ext_val, TYPE_I32, NULL, 0);
      }
      if (lval->type == TYPE_I8 && rval->type == TYPE_I32) {
        int r_left_ext_val = r_register(rctx);
        push_code(code, "  %%%d = sext i8 %%%d to i32\n", r_left_ext_val,
                  lval->reg);
        lval = new_variable(r_left_ext_val, TYPE_I32, NULL, 0);
      }

      Variable* result_val = get_calc_result_type(node->kind, lval, rval);
      char* lval_type = get_variable_type_str(lval);
      char* lval_ptype = get_ptr_variable_type_str(lval);
      char* rval_type = get_variable_type_str(rval);
      char* rval_ptype = get_ptr_variable_type_str(rval);
      char* val_type = get_variable_type_str(result_val);
      char* val_ptype = get_ptr_variable_type_str(result_val);
      char* left_val = get_variable_value_str(lval, code, rctx, is_lval);
      char* right_val = get_variable_value_str(rval, code, rctx, is_lval);

      print_debug(COL_BLUE "[generator] " COL_GREEN "[%s] " COL_RESET
                           "lval = %s, rval = %s, result_val = %s",
                  get_node_kind_name(node->kind), lval_type, rval_type,
                  val_type);

      int r_result_val;
      if ((is_pointer_like(lval) || is_pointer_like(rval)) &&
          (node->kind == ND_ADD || node->kind == ND_SUB)) {
        int r_middle;
        if (is_pointer_like(lval) && !is_pointer_like(rval)) {
          char* ptr_type = get_variable_type_str(lval->ptr_to);
          if (node->kind == ND_SUB) {
            int r_ptr_diff = r_register(rctx);
            push_code(code, "  %%%d = sub nsw %s 0, %s\n", r_ptr_diff,
                      rval_type, right_val);
            free(right_val);
            right_val = get_variable_value_str(with_reg(rval, r_ptr_diff), code,
                                               rctx, is_lval);
          }
          r_middle = r_register(rctx);
          push_code(code, "  %%%d = getelementptr inbounds %s, %s %s, %s %s\n",
                    r_middle, ptr_type, lval_type, left_val, rval_type,
                    right_val);
          free(ptr_type);
        } else if (!is_pointer_like(lval) && is_pointer_like(rval)) {
          r_middle = r_register(rctx);
          char* ptr_type = get_variable_type_str(rval->ptr_to);
          push_code(code, "  %%%d = getelementptr inbounds %s, %s %s, %s %s\n",
                    r_middle, ptr_type, rval_type, right_val, lval_type,
                    left_val);
          free(ptr_type);
        } else {
          int r_left_val_int = r_register(rctx);
          push_code(code, "  %%%d = ptrtoint %s %s to i64\n", r_left_val_int,
                    lval_type, left_val);
          int r_right_val_int = r_register(rctx);
          push_code(code, "  %%%d = ptrtoint %s %s to i64\n", r_right_val_int,
                    rval_type, right_val);
          int r_sub_result = r_register(rctx);
          push_code(code, "  %%%d = sub i64 %%%d, %%%d\n", r_sub_result,
                    r_left_val_int, r_right_val_int);
          r_middle = r_register(rctx);
          push_code(code, "  %%%d = trunc i64 %%%d to i32\n", r_middle,
                    r_sub_result);
        }
        if (is_lval) {
          r_result_val = r_register(rctx);
          push_code(code, "  %%%d = alloca %s, align %d\n", r_result_val,
                    val_type, get_variable_size(result_val));
          push_code(code, "  store %s %%%d, %s %%%d, align %d\n", val_type,
                    r_middle, val_ptype, r_result_val,
                    get_variable_size(result_val));
        } else {
          r_result_val = r_middle;
        }
      } else {
        if (node->kind == ND_ADD || node->kind == ND_SUB ||
            node->kind == ND_MUL) {
          r_result_val = r_register(rctx);
          char* op = node->kind == ND_ADD   ? "add"
                     : node->kind == ND_SUB ? "sub"
                                            : "mul";
          push_code(code, "  %%%d = %s nsw %s %s, %s\n", r_result_val, op,
                    val_type, left_val, right_val);
        } else if (node->kind == ND_DIV) {
          r_result_val = r_register(rctx);
          push_code(code, "  %%%d = sdiv %s %s, %s\n", r_result_val, val_type,
                    left_val, right_val);
        } else if (node->kind == ND_EQ || node->kind == ND_NE ||
                   node->kind == ND_LT || node->kind == ND_LE ||
                   node->kind == ND_GT || node->kind == ND_GE) {
          char* op = node->kind == ND_EQ   ? "eq"
                     : node->kind == ND_NE ? "ne"
                     : node->kind == ND_LT ? "slt"
                     : node->kind == ND_LE ? "sle"
                     : node->kind == ND_GT ? "sgt"
                                           : "sge";
          int r_middle;
          if (is_pointer_like(lval) && is_pointer_like(rval)) {
            r_middle = r_register(rctx);
            push_code(code, "  %%%d = icmp %s ptr %s, %s\n", r_middle, op,
                      left_val, right_val);
          } else if (is_pointer_like(lval) || is_pointer_like(rval)) {
            int r_ptr_int_val = r_register(rctx);
            push_code(code, "  %%%d = ptrtoint %s %s to i32\n", r_ptr_int_val,
                      is_pointer_like(lval) ? lval_type : rval_type,
                      is_pointer_like(lval) ? left_val : right_val);
            r_middle = r_register(rctx);
            push_code(code, "  %%%d = icmp %s i32 %%%d, %s\n", r_middle, op,
                      r_ptr_int_val,
                      is_pointer_like(lval) ? right_val : left_val);
          } else {
            r_middle = r_register(rctx);
            push_code(code, "  %%%d = icmp %s %s %s, %s\n", r_middle, op,
                      val_type, left_val, right_val);
          }
          if (is_boolean) {
            r_result_val = r_middle;
          } else {
            r_result_val = r_register(rctx);
            push_code(code, "  %%%d = zext i1 %%%d to %s\n", r_result_val,
                      r_middle, val_type);
          }
        } else if (node->kind == ND_AND || node->kind == ND_OR) {
          char* op = node->kind == ND_AND ? "and" : "or";
          int r_middle_l = r_register(rctx);
          push_code(code, "  %%%d = icmp ne %s %s, 0\n", r_middle_l, val_type,
                    left_val);
          int r_middle_r = r_register(rctx);
          push_code(code, "  %%%d = icmp ne %s %s, 0\n", r_middle_r, val_type,
                    right_val);
          int r_middle = r_register(rctx);
          push_code(code, "  %%%d = %s i1 %%%d, %%%d\n", r_middle, op,
                    r_middle_l, r_middle_r);
          if (is_boolean) {
            r_result_val = r_middle;
          } else {
            r_result_val = r_register(rctx);
            push_code(code, "  %%%d = zext i1 %%%d to %s\n", r_result_val,
                      r_middle, val_type);
          }
        } else {
          error("未対応の演算子です: %s", get_node_kind_name(node->kind));
          return NULL;
        }
      }

      if (result_val->value != NULL) {
        free(result_val->value);
        result_val->value = NULL;
      }
      push_variable_with_cast_if_needed(
          stack, with_reg(result_val, r_result_val), node->cast);

      free_variable(lval);
      free_variable(rval);
      free_variable(result_val);
      free(lval_type);
      free(lval_ptype);
      free(rval_type);
      free(rval_ptype);
      free(val_type);
      free(val_ptype);
      free(left_val);
      free(right_val);
      break;
    }
    default:
      error("未対応のノード種別です: %s", get_node_kind_name(node->kind));
      break;
  }

  // ブロックスコープ抜けた時の処理
  if (node->kind == ND_BLOCK || node->kind == ND_FOR) {
    for (int i = 0; i < pop_local_list->size; i++) {
      LVar* lvar = vec_at(node->locals, i);
      locals_r[lvar->offset] = vec_at(pop_local_list, i);
    }
    vec_free(pop_local_list);
  }

  return code;
}

Code* generate_struct(Variable* struct_val) {
  Code* code = init_code();
  push_code(code, "%%struct.%.*s = type { ", struct_val->len, struct_val->name);
  for (int i = 0; i < struct_val->fields->size; i++) {
    if (i != 0) push_code(code, ", ");
    LVar* field = vec_at(struct_val->fields, i);
    print_debug(COL_BLUE "[generator]" COL_RESET " field = %p", field);
    print_debug(COL_BLUE "[generator]" COL_RESET " field->var = %p",
                field->var);
    char* field_type = get_variable_type_str(field->var);
    push_code(code, "%s", field_type);
    free(field_type);
  }
  push_code(code, " }\n");
  return code;
}

Code* generate_global(LVar* var) {
  Code* code = init_code();
  char* type = get_variable_type_str(var->var);
  int size = get_variable_size(var->var);
  if (var->var->reg == -5) {
    push_code(code, "@%.*s = dso_local global %s zeroinitializer, align %d\n",
              var->len, var->name, type, size);
  } else if (var->var->reg == -6) {
    push_code(code, "@%.*s = external global %s, align %d\n", var->len,
              var->name, type, size);
  }
  free(type);
  return code;
}

Code* generate_string(Token* tok, int index) {
  Code* code = init_code();
  push_code(code,
            "@.str.%d = private unnamed_addr constant [%d x i8] c\"%.*s\\00\", "
            "align 1\n",
            index, tok->val + 1, tok->len, tok->str);
  return code;
}

Code* generate_func(Function* func) {
  Code* code = init_code();

  // 初期化処理
  vector* stack = new_vector();
  int* rctx = r_init();
  return_type = func->ret;

  // 関数のローカル構造体の宣言
  print_debug(COL_BLUE "[generator]" COL_RESET " func->structs->size = %d",
              func->structs->size);
  for (int i = 0; i < func->structs->size; i++) {
    Variable* var = vec_at(func->structs, i);
    print_debug(COL_BLUE "[generator]" COL_RESET " func->structs[%d] = %.*s", i,
                var->len, var->name);
    merge_code(code, generate_struct(var));
  }

  char* ret_type = get_variable_type_str(func->ret);
  if (func->is_proto) {
    push_code(code, "declare ");
  } else {
    push_code(code, "define ");
  }
  push_code(code, "dso_local %s @%.*s(", ret_type, func->len, func->name);
  free(ret_type);
  int* args = calloc(func->argc, sizeof(int));
  for (int i = 0; i < func->argc; i++) {
    if (i != 0) push_code(code, ", ");
    LVar* arg = vec_at(func->locals, i);
    char* type = get_variable_type_str(arg->var);
    int r_arg = r_register(rctx);
    push_code(code, "%s noundef %%%d", type, r_arg);
    args[i] = r_arg;
    free(type);
  }
  if (func->have_va_arg) {
    if (func->argc > 0) push_code(code, ", ");
    push_code(code, "...");
  }
  push_code(code, ") #0");

  if (func->is_proto) {
    push_code(code, "\n");
    return code;
  }

  push_code(code, " {\n");

  r_register(rctx);

  // ローカル変数の初期化
  print_debug(COL_BLUE "[generator]" COL_RESET " func->locals->size = %d",
              func->locals->size);
  Variable** locals_r = calloc(1024, sizeof(Variable*));
  for (int i = 0; i < func->argc; i++) {
    LVar* arg = vec_at(func->locals, i);
    print_debug(COL_BLUE "[generator]" COL_RESET
                         " func->locals[%d] = %.*s (%s)",
                i, arg->len, arg->name, get_variable_type_str(arg->var));
    char* var_type = get_variable_type_str(arg->var);
    char* var_ptype = get_ptr_variable_type_str(arg->var);
    int var_size = get_variable_size(arg->var);
    int r = r_register(rctx);
    push_code(code, "  ; %.*s (arg)\n", arg->len, arg->name);
    push_code(code, "  %%%d = alloca %s, align %d\n", r, var_type, var_size);
    push_code(code, "  store %s %%%d, %s %%%d, align %d\n", var_type, args[i],
              var_ptype, r, var_size);
    locals_r[i] = with_reg(arg->var, r);
    free(var_type);
    free(var_ptype);
  }
  for (int i = func->argc; i < func->locals->size; i++) {
    LVar* lvar = vec_at(func->locals, i);
    print_debug(COL_BLUE "[generator]" COL_RESET
                         " func->locals[%d] = %.*s (%s)",
                i, lvar->len, lvar->name, get_variable_type_str(lvar->var));
    char* var_type = get_variable_type_str(lvar->var);
    int var_size = get_variable_size(lvar->var);
    int r = r_register(rctx);
    push_code(code, "  ; %.*s (local)\n", lvar->len, lvar->name);
    push_code(code, "  %%%d = alloca %s, align %d\n", r, var_type, var_size);
    print_debug("with_reg before (lvar->var = %p)", lvar->var);
    locals_r[i] = with_reg(lvar->var, r);
    print_debug("with_reg after");
    free(var_type);
  }

  // 関数本体の生成
  for (int i = 0; i < func->body->size; i++) {
    continue_labels = new_vector();
    break_labels = new_vector();
    Node* node = vec_at(func->body, i);
    if (node == NULL) continue;
    print_debug(COL_BLUE "[generator]" COL_RESET " func->body[%d] = %d", i,
                node->kind);
    push_code(code, "  ; ");
    merge_code(code, print_node(node));
    push_code(code, "\n");
    merge_code(code, generate_node(node, stack, locals_r, rctx, false));
    vec_free(continue_labels);
    vec_free(break_labels);
  }

  if (memcmp(func->name, "main", 4) == 0) {
    push_code(code, "  ret i32 0\n");
  } else if (func->ret->type == TYPE_VOID) {
    push_code(code, "  ret void\n");
  } else {
    char* ret_type = get_variable_type_str(func->ret);
    char* ret_ptype = get_ptr_variable_type_str(func->ret);
    int ret_size = get_variable_size(func->ret);
    int r_ret_var = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_ret_var, ret_type,
              ret_size);
    int r_ret_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_ret_val,
              ret_type, ret_ptype, r_ret_var, ret_size);
    push_code(code, "  ret %s %%%d\n", ret_type, r_ret_val);
    free(ret_type);
    free(ret_ptype);
  }

  push_code(code, "}\n");

  // 後処理
  vec_free(stack);
  r_free(rctx);
  for (int i = 0; i < func->locals->size; i++) {
    free_variable(locals_r[i]);
  }
  free(locals_r);
  free_function(func);

  return code;
}

Code* generate_header() {
  Code* code = init_code();
  push_code(
      code,
      "source_filename = \"%s\"\n"
      "target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"\n"
      "target triple = \"x86_64-pc-linux-gnu\"\n"
      "\n",
      filename);
  return code;
}

Code* generate_lib_functions() {
  Code* code = init_code();
  char* lib_code = read_file("src/lib.ll");
  push_code(code, "%s", lib_code);

  return code;
}

Code* generate(Program* program) {
  Code* code = init_code();
  merge_code(code, generate_header());

  print_debug(COL_BLUE "[generator]" COL_RESET " program->structs->size = %d",
              program->structs->size);
  for (int i = 0; i < program->structs->size; i++) {
    Variable* strcut_val = vec_at(program->structs, i);
    print_debug(COL_BLUE "[generator]" COL_RESET " program->structs[%d] = %.*s",
                i, strcut_val->len, strcut_val->name);
    merge_code(code, generate_struct(strcut_val));
  }

  print_debug(COL_BLUE "[generator]" COL_RESET " program->globals->size = %d",
              program->globals->size);
  globals = calloc(program->globals->size, sizeof(Variable*));
  for (int i = 0; i < program->globals->size; i++) {
    LVar* var = vec_at(program->globals, i);
    globals[i] = var->var;
    print_debug(COL_BLUE "[generator]" COL_RESET " program->globals[%d] = %.*s",
                i, var->len, var->name);
    if (var->var->value != NULL) continue;
    merge_code(code, generate_global(var));
  }

  print_debug(COL_BLUE "[generator]" COL_RESET " program->strings->size = %d",
              program->strings->size);
  for (int i = 0; i < program->strings->size; i++) {
    Token* tok = vec_at(program->strings, i);
    print_debug(COL_BLUE "[generator]" COL_RESET " program->strings[%d] = %.*s",
                i, tok->len, tok->str);
    merge_code(code, generate_string(tok, i));
  }

  // コード生成
  print_debug(COL_BLUE "[generator]" COL_RESET " program->functions->size = %d",
              program->functions->size);
  for (int i = 0; i < program->functions->size; i++) {
    Function* func = vec_at(program->functions, i);
    print_debug(COL_BLUE "[generator]" COL_RESET
                         " program->functions[%d] = %.*s",
                i, func->len, func->name);
    merge_code(code, generate_func(func));
  }

  push_code(code, "\n");

  merge_code(code, generate_lib_functions());

  return code;
}
