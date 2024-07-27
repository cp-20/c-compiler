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

Variable* get_variable(Variable** locals, int offset) {
  if (offset >= 0) return locals[offset];
  return globals[-offset - 1];
}

// 左辺値のポインタ (レジスタ) を返す
Variable* gen_lval(Code* code, Node* node, vector* stack, Variable** locals_r,
                   rctx rctx) {
  if (node->kind == ND_LVAR) {
    return get_variable(locals_r, node->offset);
  }

  if (node->kind == ND_DEREF) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
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
    return lvar;
  }

  if (node->kind == ND_ACCESS) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* lvar_val = pop_variable(stack);
    char* type = get_variable_type_str(lvar_val);
    char* ptype = get_ptr_variable_type_str(lvar_val);
    int r_field = r_register(rctx);
    push_code(code,
              "  %%%d = getelementptr inbounds %s, %s %%%d, i32 0, i32 %d\n",
              r_field, type, ptype, lvar_val->reg, node->rhs->val);
    LVar* field = vec_at(lvar_val->fields, node->rhs->val);
    Variable* field_var = with_reg(field->var, r_field);
    return field_var;
  }

  error("代入の左辺値が変数ではありません");
  return NULL;
}

void push_variable_with_cast_if_needed(vector* stack, Variable* var,
                                       Variable* cast) {
  if (cast != NULL) {
    push_variable(stack, with_reg(cast, var->reg));
  } else {
    push_variable(stack, var);
  }
}

Code* generate_node(Node* node, vector* stack, Variable** locals_r, rctx rctx) {
  print_debug(COL_BLUE "[generator]" COL_RESET " generate_node %d", node->kind);

  Code* code = init_code();
  push_code(code, "  ; generate %d -> ", node->kind);
  merge_code(code, print_node(node));
  push_code(code, "\n");

  if (node->kind == ND_NUM) {
    // 数字の場合はそのままスタックに積む
    int r_num = r_register(rctx);
    push_code(code, "  %%%d = alloca i32, align 4\n", r_num);
    if (node->cast != NULL && node->cast->type == TYPE_PTR &&
        node->cast->ptr_to->type == TYPE_VOID && node->val == 0) {
      push_code(code, "  store ptr null, ptr %%%d, align 4\n", r_num);
    } else {
      push_code(code, "  store i32 %d, i32* %%%d, align 4\n", node->val, r_num);
    }
    if (node->cast != NULL) {
      push_variable_with_cast_if_needed(stack, with_reg(node->cast, r_num),
                                        node->cast);
    } else {
      push_variable_with_cast_if_needed(
          stack, new_variable(r_num, TYPE_I32, NULL, 0), node->cast);
    }
    return code;
  } else if (node->kind == ND_STRING) {
    int r_string = r_register(rctx);
    push_code(code, "  %%%d = alloca i8*, align 8\n", r_string);
    push_code(code, "  store i8* @.str.%d, i8** %%%d, align 8\n", node->offset,
              r_string);
    Variable* var =
        new_variable(r_string, TYPE_PTR, new_variable(-1, TYPE_I8, NULL, 0), 0);
    push_variable_with_cast_if_needed(stack, var, node->cast);
    return code;
  } else if (node->kind == ND_RETURN) {
    // returnの場合は値を計算して返す
    if (node->lhs == NULL) {
      push_code(code, "  ret void\n");
      r_register(rctx);
      return code;
    }
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* val = get_last_variable(stack);
    char* val_type = get_variable_type_str(val);
    char* val_ptype = get_ptr_variable_type_str(val);
    int val_size = get_variable_size(val);
    int r_result_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_result_val,
              val_type, val_ptype, val->reg, val_size);
    push_code(code, "  ret %s %%%d\n", val_type, r_result_val);
    // なぜかよくわからないけどレジスタを1個空けると上手く行く
    r_register(rctx);
    return code;
  } else if (node->kind == ND_LVAR) {
    Variable* var = get_variable(locals_r, node->offset);
    print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_LVAR] " COL_RESET
                         "generate var");
    if (var->value != NULL) {
      merge_code(code, generate_node(new_node_num(*var->value), stack, locals_r,
                                     rctx));
      return code;
    }
    if (var->type == TYPE_ARRAY) {
      int r_var = r_register(rctx);
      push_code(code,
                "  %%%d = getelementptr inbounds %s, ptr %%%d, i64 0, i64 0\n",
                r_var, get_variable_type_str(var), var->reg);
      push_variable_with_cast_if_needed(
          stack, new_variable(r_var, TYPE_PTR, var->ptr_to, 0), node->cast);
    } else {
      push_variable_with_cast_if_needed(stack, var, node->cast);
    }
    return code;
  } else if (node->kind == ND_ASSIGN) {
    // 代入の場合は右辺を計算して左辺に代入する
    Variable* lvar = gen_lval(code, node->lhs, stack, locals_r, rctx);
    if (node->rhs->kind == ND_CALL) {
      free(node->rhs->call->ret);
      node->rhs->call->ret = lvar;
    }
    merge_code(code, generate_node(node->rhs, stack, locals_r, rctx));

    Variable* rhs = pop_variable(stack);
    if (!is_same_type(lvar, rhs)) {
      error("代入の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s",
            get_variable_type_str(lvar), get_variable_type_str(rhs));
    }
    char* type = get_variable_type_str(rhs);
    char* ptype = get_ptr_variable_type_str(rhs);
    int size = get_variable_size(rhs);
    int r_right_val = r_register(rctx);
    if (rhs->reg >= 0) {
      push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_right_val,
                type, ptype, rhs->reg, size);
    } else {
      push_code(code, "  %%%d = load %s, %s @%.*s, align %d\n", r_right_val,
                type, ptype, rhs->len, rhs->name, size);
    }
    if (node->lhs->offset >= 0) {
      push_code(code, "  store %s %%%d, %s %%%d, align %d\n", type, r_right_val,
                ptype, lvar->reg, size);
    } else {
      push_code(code, "  store %s %%%d, %s @%.*s, align %d\n", type,
                r_right_val, ptype, lvar->len, lvar->name, size);
    }
    push_variable_with_cast_if_needed(stack, with_reg(rhs, lvar->reg),
                                      node->cast);
    return code;
  } else if (node->kind == ND_IF) {
    // ifの場合は条件式を計算して条件によって分岐する
    // 条件式を計算する
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));

    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    char* cond_ptype = get_ptr_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_cond_val,
              cond_type, cond_ptype, cond->reg, cond_size);
    int r_cond_bool = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_cond_bool, cond_type,
              r_cond_val);

    int r_then_label = r_register(rctx);

    Code* block = init_code();
    push_code(block, "%d:\n", r_then_label);
    merge_code(block, generate_node(node->rhs, stack, locals_r, rctx));

    int r_next_label = r_register(rctx);

    if (node->extra != NULL) {
      Code* sub_block = init_code();
      push_code(sub_block, "%d:\n", r_next_label);
      merge_code(sub_block, generate_node(node->extra, stack, locals_r, rctx));
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
    return code;
  } else if (node->kind == ND_WHILE) {
    // whileの場合は条件式を計算して条件によってループする

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
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    char* cond_ptype = get_ptr_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_cond_val,
              cond_type, cond_ptype, cond->reg, cond_size);

    int r_cond_bool = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_cond_bool, cond_type,
              r_cond_val);

    Code* break_block = init_code();
    int r_break_label = r_register(rctx);
    int* r_break_label_ptr = malloc(sizeof(int));
    *r_break_label_ptr = r_break_label;
    vec_push_last(break_labels, r_break_label_ptr);
    push_code(break_block, "%d:\n", r_break_label);

    Code* block = init_code();

    int r_next_label = r_register(rctx);
    push_code(block, "%d:\n", r_next_label);
    merge_code(block, generate_node(node->rhs, stack, locals_r, rctx));
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
    return code;
  } else if (node->kind == ND_FOR) {
    // forの場合は初期化式、条件式、ループ内式の順に実行する

    //   初期化式
    // start_label:
    //   条件式の評価
    //   br i1 %cond, label %next_label, label %end_label
    // break_label:
    //   br label %end_label
    // continue_label:
    //   ループ内の処理
    // next_label:
    //   ループ内の処理
    //   ループ内式
    //   br label %start_label
    // end_label:

    // 初期化式
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));

    int r_start_label = r_register(rctx);
    push_code(code, "  br label %%%d\n", r_start_label);
    push_code(code, "%d:\n", r_start_label);

    // 条件式を計算する
    merge_code(code, generate_node(node->rhs, stack, locals_r, rctx));
    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    char* cond_ptype = get_ptr_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_cond_val,
              cond_type, cond_ptype, cond->reg, cond_size);

    int r_cond_bool = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_cond_bool, cond_type,
              r_cond_val);

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
    merge_code(block, generate_node(node->extra2, stack, locals_r, rctx));
    int r_continue_dist_label = r_register(rctx);
    push_code(continue_block, "  br label %%%d\n", r_continue_dist_label);
    push_code(block, "  br label %%%d\n", r_continue_dist_label);
    push_code(block, "%d:\n", r_continue_dist_label);
    merge_code(block, generate_node(node->extra, stack, locals_r, rctx));
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
    return code;
  } else if (node->kind == ND_BLOCK || node->kind == ND_GROUP) {
    // ブロックの場合は各文を順に生成する
    for (int i = 0; i < node->stmts->size; i++) {
      Node* stmt = vec_at(node->stmts, i);
      merge_code(code, generate_node(stmt, stack, locals_r, rctx));
    }
    return code;
  } else if (node->kind == ND_CALL) {
    // 関数呼び出しの場合は関数を呼び出す

    // 引数を計算する
    print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_CALL] " COL_RESET
                         "node->call->args->size = %d",
                node->call->args->size);
    for (int i = 0; i < node->call->args->size; i++) {
      Node* arg = vec_at(node->call->args, i);
      print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_CALL] " COL_RESET
                           "node->call->args[%d] = %d",
                  i, arg->kind);
      merge_code(code, generate_node(arg, stack, locals_r, rctx));
    }
    Variable args[node->call->args->size];
    for (int i = 0; i < node->call->args->size; i++) {
      int j = node->call->args->size - i - 1;
      Variable* arg = pop_variable(stack);
      char* arg_type = get_variable_type_str(arg);
      char* arg_ptype = get_ptr_variable_type_str(arg);
      int arg_size = get_variable_size(arg);
      int r_arg_val = r_register(rctx);
      if (arg->reg >= 0) {
        push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_arg_val,
                  arg_type, arg_ptype, arg->reg, arg_size);
      } else {
        push_code(code, "  %%%d = load %s, %s @%.*s, align %d\n", r_arg_val,
                  arg_type, arg_ptype, arg->len, arg->name, arg_size);
      }
      args[j].reg = r_arg_val;
      args[j].type = arg->type;
      args[j].ptr_to = arg->ptr_to;
      args[j].array_size = arg->array_size;
    }

    // 関数呼び出し本体
    int r_result_val = r_register(rctx);
    // とりあえず関数の戻り値はi32固定
    char* return_type = get_variable_type_str(node->call->ret);
    if (node->cast != NULL) {
      return_type = get_variable_type_str(node->cast);
    }
    char* return_ptype = get_ptr_variable_type_str(node->call->ret);
    push_code(code, "  %%%d = call %s @%.*s(", r_result_val, return_type,
              node->call->len, node->call->name);
    for (int i = 0; i < node->call->args->size; i++) {
      if (i > 0) push_code(code, ", ");
      push_code(code, "%s noundef %%%d", get_variable_type_str(&args[i]),
                args[i].reg);
    }
    push_code(code, ")\n");

    // 結果をスタックに積む
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align 4\n", r_result, return_type);
    push_code(code, "  store %s %%%d, %s %%%d, align 4\n", return_type,
              r_result_val, return_ptype, r_result);
    push_variable_with_cast_if_needed(
        stack, with_reg(node->call->ret, r_result), node->cast);

    return code;
  } else if (node->kind == ND_INCR || node->kind == ND_DECR) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* var = pop_variable(stack);
    char* var_type = get_variable_type_str(var);
    char* var_ptype = get_ptr_variable_type_str(var);
    int var_size = get_variable_size(var);
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_result, var_type,
              var_size);
    int r_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_val, var_type,
              var_ptype, var->reg, var_size);
    push_code(code, "  store %s %%%d, %s %%%d, align %d\n", var_type, r_val,
              var_ptype, r_result, var_size);
    push_variable_with_cast_if_needed(stack, with_reg(var, r_result),
                                      node->cast);

    NodeKind op_type = node->kind == ND_INCR ? ND_ADD : ND_SUB;
    Node* op_node = new_node(ND_ASSIGN, node->lhs,
                             new_node(op_type, node->lhs, new_node_num(1)));
    merge_code(code, generate_node(op_node, stack, locals_r, rctx));
    pop_variable(stack);
    return code;
  } else if (node->kind == ND_REF) {
    // 参照の場合は左辺値のポインタをスタックに積む
    Variable* lvar = gen_lval(code, node->lhs, stack, locals_r, rctx);
    int r_result = r_register(rctx);
    Variable* lvar_ptr = new_variable(r_result, TYPE_PTR, lvar, -1);
    char* lvar_ptr_type = get_variable_type_str(lvar_ptr);
    char* lvar_ptr_ptype = get_ptr_variable_type_str(lvar_ptr);
    push_code(code, "  %%%d = alloca %s, align 8\n", r_result, lvar_ptr_type);
    push_code(code, "  store %s %%%d, %s %%%d, align 8\n", lvar_ptr_type,
              lvar->reg, lvar_ptr_ptype, r_result);
    push_variable_with_cast_if_needed(stack, lvar_ptr, node->cast);
    return code;
  } else if (node->kind == ND_DEREF) {
    // 間接参照の場合はポインタの値をスタックに積む
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* rhs = pop_variable(stack);
    if (!is_pointer_like(rhs)) {
      error("間接参照演算子の右辺値がポインタではありません");
    }
    char* rhs_type = get_variable_type_str(rhs);
    char* rhs_ptype = get_ptr_variable_type_str(rhs);
    int r_ptr_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align 8\n", r_ptr_val, rhs_type,
              rhs_ptype, rhs->reg);
    push_variable_with_cast_if_needed(stack, with_reg(rhs->ptr_to, r_ptr_val),
                                      node->cast);
    return code;
  } else if (node->kind == ND_ACCESS) {
    // フィールドアクセスの場合は構造体の値をスタックに積む
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* lvar = pop_variable(stack);
    char* type = get_variable_type_str(lvar);
    char* ptype = get_ptr_variable_type_str(lvar);
    int r_field = r_register(rctx);
    push_code(code,
              "  %%%d = getelementptr inbounds %s, %s %%%d, i32 0, i32 %d\n",
              r_field, type, ptype, lvar->reg, node->rhs->val);
    LVar* field = vec_at(lvar->fields, node->rhs->val);
    Variable* field_var = with_reg(field->var, r_field);
    push_variable_with_cast_if_needed(stack, field_var, node->cast);
    return code;
  } else if (node->kind == ND_TERNARY) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));

    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    char* cond_ptype = get_ptr_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_cond_val,
              cond_type, cond_ptype, cond->reg, cond_size);
    int r_cond_bool = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_cond_bool, cond_type,
              r_cond_val);

    int r_result = r_register(rctx);
    char* result_type = get_variable_type_str(node->cast);
    int result_size = get_variable_size(node->cast);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_result, result_type,
              result_size);
    int r_then_label = r_register(rctx);

    Code* block = init_code();
    push_code(block, "%d:\n", r_then_label);
    print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_TERNARY] " COL_RESET
                         "generate ternary 1");
    merge_code(block, generate_node(node->rhs->lhs, stack, locals_r, rctx));
    print_debug(COL_BLUE "[generator] " COL_GREEN "[ND_TERNARY] " COL_RESET
                         "generate ternary 2");
    Variable* lhs_result = pop_variable(stack);
    char* lhs_type = get_variable_type_str(lhs_result);
    char* lhs_ptype = get_ptr_variable_type_str(lhs_result);
    int lhs_size = get_variable_size(lhs_result);
    int lhs_val = r_register(rctx);
    push_code(block, "  %%%d = load %s, %s %%%d, align %d\n", lhs_val, lhs_type,
              lhs_ptype, lhs_result->reg, lhs_size);
    push_code(block, "  store %s %%%d, %s %%%d, align %d\n", lhs_type, lhs_val,
              lhs_ptype, r_result, lhs_size);

    int r_next_label = r_register(rctx);

    Code* sub_block = init_code();
    push_code(sub_block, "%d:\n", r_next_label);
    merge_code(sub_block, generate_node(node->rhs->rhs, stack, locals_r, rctx));
    Variable* rhs_result = pop_variable(stack);
    char* rhs_type = get_variable_type_str(rhs_result);
    char* rhs_ptype = get_ptr_variable_type_str(rhs_result);
    int rhs_size = get_variable_size(rhs_result);
    int rhs_val = r_register(rctx);
    push_code(sub_block, "  %%%d = load %s, %s %%%d, align %d\n", rhs_val,
              rhs_type, rhs_ptype, rhs_result->reg, rhs_size);
    push_code(sub_block, "  store %s %%%d, %s %%%d, align %d\n", rhs_type,
              rhs_val, rhs_ptype, r_result, rhs_size);
    int r_final_label = r_register(rctx);
    push_code(sub_block, "  br label %%%d\n", r_final_label);
    push_code(block, "  br label %%%d\n", r_final_label);

    merge_code(block, sub_block);
    push_code(block, "%d:\n", r_final_label);

    push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
              r_then_label, r_next_label);
    merge_code(code, block);
    push_variable(stack, with_reg(node->cast, r_result));
    return code;
  } else if (node->kind == ND_CONTINUE) {
    if (continue_labels->size == 0) {
      error("continue文がループの外で使われています");
    }
    int* r_continue_label_ptr = vec_last(continue_labels);
    push_code(code, "  br label %%%d\n", *r_continue_label_ptr);
    r_register(rctx);
    return code;
  } else if (node->kind == ND_BREAK) {
    if (break_labels->size == 0) {
      error("break文がループの外で使われています");
    }
    int* r_break_label_ptr = vec_last(break_labels);
    push_code(code, "  br label %%%d\n", *r_break_label_ptr);
    r_register(rctx);
    return code;
  }

  // 演算子の場合は左右のノードを先に計算する
  merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
  merge_code(code, generate_node(node->rhs, stack, locals_r, rctx));

  // スタックから2つ取り出す
  Variable* rval = pop_variable(stack);
  Variable* lval = pop_variable(stack);

  if (lval->type == TYPE_I32 && rval->type == TYPE_I8) {
    int r_right_val = r_register(rctx);
    push_code(code, "  %%%d = load i8, i8* %%%d, align 1\n", r_right_val,
              rval->reg);
    int r_right_ext_val = r_register(rctx);
    push_code(code, "  %%%d = sext i8 %%%d to i32\n", r_right_ext_val,
              r_right_val);
    int r_right_i32 = r_register(rctx);
    push_code(code, "  %%%d = alloca i32, align 4\n", r_right_i32);
    push_code(code, "  store i32 %%%d, i32* %%%d, align 4\n", r_right_ext_val,
              r_right_i32);
    rval = new_variable(r_right_i32, TYPE_I32, NULL, 0);
  }

  Variable* result_val = get_calc_result_type(node->kind, lval, rval);
  char* lval_type = get_variable_type_str(lval);
  char* lval_ptype = get_ptr_variable_type_str(lval);
  int lval_size = get_variable_size(lval);
  char* rval_type = get_variable_type_str(rval);
  char* rval_ptype = get_ptr_variable_type_str(rval);
  int rval_size = get_variable_size(rval);
  char* val_type = get_variable_type_str(result_val);
  char* val_ptype = get_ptr_variable_type_str(result_val);
  int val_size = get_variable_size(result_val);
  int r_left_val = r_register(rctx);
  int r_right_val = r_register(rctx);
  if (lval->reg >= 0) {
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_left_val,
              lval_type, lval_ptype, lval->reg, lval_size);
  } else {
    push_code(code, "  %%%d = load %s, %s @%.*s, align %d\n", r_left_val,
              lval_type, lval_ptype, lval->len, lval->name, lval_size);
  }
  if (rval->reg >= 0) {
    push_code(code, "  %%%d = load %s, %s %%%d, align %d\n", r_right_val,
              rval_type, rval_ptype, rval->reg, rval_size);
  } else {
    push_code(code, "  %%%d = load %s, %s @%.*s, align %d\n", r_right_val,
              rval_type, rval_ptype, rval->len, rval->name, rval_size);
  }

  if (is_pointer_like(lval) || is_pointer_like(rval)) {
    int r_result_val = r_register(rctx);
    if (is_pointer_like(lval) && !is_pointer_like(rval)) {
      char* ptr_type = get_variable_type_str(lval->ptr_to);
      int r_ptr_diff = r_right_val;
      if (node->kind == ND_SUB) {
        r_ptr_diff = r_register(rctx) - 1;
        r_result_val++;
        push_code(code, "  %%%d = sub nsw %s 0, %%%d\n", r_ptr_diff, rval_type,
                  r_right_val);
      }
      push_code(code, "  %%%d = getelementptr inbounds %s, %s %%%d, %s %%%d\n",
                r_result_val, ptr_type, lval_type, r_left_val, rval_type,
                r_ptr_diff);
    } else if (!is_pointer_like(lval) && is_pointer_like(rval)) {
      char* ptr_type = get_variable_type_str(rval->ptr_to);
      push_code(code, "  %%%d = getelementptr inbounds %s, %s %%%d, %s %%%d\n",
                r_result_val, ptr_type, rval_type, r_right_val, lval_type,
                r_left_val);
    } else {
      int r_left_val_int = r_register(rctx) - 1;
      r_result_val++;
      push_code(code, "  %%%d = ptrtoint %s %%%d to i64\n", r_left_val_int,
                lval_type, r_left_val);
      int r_right_val_int = r_register(rctx) - 1;
      r_result_val++;
      push_code(code, "  %%%d = ptrtoint %s %%%d to i64\n", r_right_val_int,
                rval_type, r_right_val);
      int r_sub_result = r_register(rctx) - 1;
      r_result_val++;
      push_code(code, "  %%%d = sub i64 %%%d, %%%d\n", r_sub_result,
                r_left_val_int, r_right_val_int);
      push_code(code, "  %%%d = trunc i64 %%%d to i32\n", r_result_val,
                r_sub_result);
    }
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_result, val_type,
              val_size);
    push_code(code, "  store %s %%%d, %s %%%d, align %d\n", val_type,
              r_result_val, val_ptype, r_result, val_size);

    push_variable_with_cast_if_needed(stack, with_reg(result_val, r_result),
                                      node->cast);
    return code;
  }

  // 計算する
  int r_result_val = r_register(rctx);
  if (node->kind == ND_ADD || node->kind == ND_SUB || node->kind == ND_MUL) {
    char op[4];
    if (node->kind == ND_ADD) {
      sprintf(op, "add");
    } else if (node->kind == ND_SUB) {
      sprintf(op, "sub");
    } else if (node->kind == ND_MUL) {
      sprintf(op, "mul");
    }
    push_code(code, "  %%%d = %s nsw %s %%%d, %%%d\n", r_result_val, op,
              val_type, r_left_val, r_right_val);
  } else if (node->kind == ND_DIV) {
    push_code(code, "  %%%d = sdiv %s %%%d, %%%d\n", r_result_val, val_type,
              r_left_val, r_right_val);
  } else if (node->kind == ND_EQ || node->kind == ND_NE ||
             node->kind == ND_LT || node->kind == ND_LE ||
             node->kind == ND_GT || node->kind == ND_GE) {
    char op[4];
    if (node->kind == ND_EQ) {
      sprintf(op, "eq");
    } else if (node->kind == ND_NE) {
      sprintf(op, "ne");
    } else if (node->kind == ND_LT) {
      sprintf(op, "slt");
    } else if (node->kind == ND_LE) {
      sprintf(op, "sle");
    } else if (node->kind == ND_GT) {
      sprintf(op, "sgt");
    } else if (node->kind == ND_GE) {
      sprintf(op, "sge");
    }
    int r_middle = r_register(rctx) - 1;
    r_result_val++;
    if (is_pointer_like(lval) || is_pointer_like(rval)) {
      push_code(code, "  %%%d = icmp %s ptr %%%d, %%%d\n", r_middle, op,
                r_left_val, r_right_val);
    } else {
      push_code(code, "  %%%d = icmp %s %s %%%d, %%%d\n", r_middle, op,
                val_type, r_left_val, r_right_val);
    }
    push_code(code, "  %%%d = zext i1 %%%d to %s\n", r_result_val, r_middle,
              val_type);
  } else if (node->kind == ND_AND || node->kind == ND_OR) {
    char op[4];
    if (node->kind == ND_AND) {
      sprintf(op, "and");
    } else if (node->kind == ND_OR) {
      sprintf(op, "or");
    }
    int r_middle_l = r_register(rctx) - 1;
    int r_middle_r = r_register(rctx) - 1;
    int r_middle = r_register(rctx) - 1;
    r_result_val += 3;
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_middle_l, val_type,
              r_left_val);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_middle_r, val_type,
              r_right_val);
    push_code(code, "  %%%d = %s i1 %%%d, %%%d\n", r_middle, op, r_middle_l,
              r_middle_r);
    push_code(code, "  %%%d = zext i1 %%%d to %s\n", r_result_val, r_middle,
              val_type);
  }

  // 結果を再びスタックに積む
  int r_result = r_register(rctx);
  push_code(code, "  %%%d = alloca %s, align %d\n", r_result, val_type,
            val_size);
  push_code(code, "  store %s %%%d, %s %%%d, align %d\n", val_type,
            r_result_val, val_ptype, r_result, val_size);
  push_variable_with_cast_if_needed(stack, with_reg(result_val, r_result),
                                    node->cast);

  return code;
}

Code* generate_struct(Variable* struct_val) {
  Code* code = init_code();
  push_code(code, "%%struct.%.*s = type { ", struct_val->len, struct_val->name);
  for (int i = 0; i < struct_val->fields->size; i++) {
    if (i != 0) push_code(code, ", ");
    LVar* field = vec_at(struct_val->fields, i);
    char* field_type = get_variable_type_str(field->var);
    push_code(code, "%s", field_type);
  }
  push_code(code, " }\n");
  return code;
}

Code* generate_global(LVar* var) {
  Code* code = init_code();
  char* type = get_variable_type_str(var->var);
  int size = get_variable_size(var->var);
  if (var->var->reg == -1) {
    push_code(code, "@%.*s = dso_local global %s zeroinitializer, align %d\n",
              var->len, var->name, type, size);
  } else if (var->var->reg == -2) {
    push_code(code, "@%.*s = external global %s, align %d\n", var->len,
              var->name, type, size);
  }
  return code;
}

Code* generate_string(Token* tok, int index) {
  Code* code = init_code();
  push_code(code,
            "@.str.%d = private unnamed_addr constant [%d x i8] c\"%.*s\\00\", "
            "align 1\n",
            index, tok->len + 1, tok->len, tok->str);
  return code;
}

Code* generate_func(Function* func) {
  Code* code = init_code();

  // 初期化処理
  vector* stack = new_vector();
  rctx rctx = r_init();

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
  int args[func->argc];
  for (int i = 0; i < func->argc; i++) {
    if (i != 0) push_code(code, ", ");
    LVar* arg = vec_at(func->locals, i);
    char* type = get_variable_type_str(arg->var);
    int r_arg = r_register(rctx);
    push_code(code, "%s noundef %%%d", type, r_arg);
    args[i] = r_arg;
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
  Variable** locals_r = calloc(func->locals->size, sizeof(Variable*));
  for (int i = 0; i < func->argc; i++) {
    LVar* arg = vec_at(func->locals, i);
    print_debug(COL_BLUE "[generator]" COL_RESET " func->locals[%d] = %.*s", i,
                arg->len, arg->name);
    char* var_type = get_variable_type_str(arg->var);
    char* var_ptype = get_ptr_variable_type_str(arg->var);
    int var_size = get_variable_size(arg->var);
    int r = r_register(rctx);
    push_code(code, "  ; %.*s (arg)\n", arg->len, arg->name);
    push_code(code, "  %%%d = alloca %s, align %d\n", r, var_type, var_size);
    push_code(code, "  store %s %%%d, %s %%%d, align %d\n", var_type, args[i],
              var_ptype, r, var_size);
    locals_r[i] = with_reg(arg->var, r);
  }
  for (int i = func->argc; i < func->locals->size; i++) {
    LVar* lvar = vec_at(func->locals, i);
    print_debug(COL_BLUE "[generator]" COL_RESET " func->locals[%d] = %.*s", i,
                lvar->len, lvar->name);
    char* var_type = get_variable_type_str(lvar->var);
    int var_size = get_variable_size(lvar->var);
    int r = r_register(rctx);
    push_code(code, "  ; %.*s (local)\n", lvar->len, lvar->name);
    push_code(code, "  %%%d = alloca %s, align %d\n", r, var_type, var_size);
    locals_r[i] = with_reg(lvar->var, r);
  }

  // 関数本体の生成
  for (int i = 0; i < func->body->size; i++) {
    continue_labels = new_vector();
    break_labels = new_vector();
    Node* node = vec_at(func->body, i);
    if (node == NULL) continue;
    push_code(code, "  ; ");
    merge_code(code, print_node(node));
    push_code(code, "\n");
    merge_code(code, generate_node(node, stack, locals_r, rctx));
    vec_free(continue_labels);
    vec_free(break_labels);
  }

  if (memcmp(func->name, "main", 4) == 0) {
    push_code(code, "  ret i32 0\n");
  }
  if (func->ret->type == TYPE_VOID) {
    push_code(code, "  ret void\n");
  }

  push_code(code, "}\n");

  // 後処理
  vec_free(stack);
  r_free(rctx);

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
