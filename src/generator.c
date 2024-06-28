#include "generator.h"

#include <stdio.h>

#include "code.h"
#include "error.h"
#include "llvm.h"
#include "parser.h"
#include "variable.h"
#include "vector.h"

// 左辺値のポインタ (レジスタ) を返す
Variable* gen_lval(Code* code, Node* node, vector* stack, Variable** locals_r,
                   rctx rctx) {
  if (node->kind != ND_LVAR && node->kind != ND_DEREF)
    error("代入の左辺値が変数ではありません");

  if (node->kind == ND_LVAR) {
    return locals_r[node->offset];
  }

  if (node->kind == ND_DEREF) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* lvar_val = pop_variable(stack);
    if (!is_pointer_like(lvar_val)) {
      error("間接参照演算子の右辺値がポインタではありません");
    }
    char* rhs_type = get_variable_type_str(lvar_val);
    int r_ptr_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align 8\n", r_ptr_val,
              rhs_type, rhs_type, lvar_val->reg);
    Variable* lvar = with_reg(lvar_val->ptr_to, r_ptr_val);
    return lvar;
  }

  error("代入の左辺値が変数ではありません");
  return locals_r[0];
}

Code* generate_node(Node* node, vector* stack, Variable** locals_r, rctx rctx) {
  Code* code = init_code();

  if (node->kind == ND_NUM) {
    // 数字の場合はそのままスタックに積む
    int r_num = r_register(rctx);
    push_code(code, "  %%%d = alloca i32, align 4\n", r_num);
    push_code(code, "  store i32 %d, i32* %%%d, align 4\n", node->val, r_num);
    push_variable(stack, new_variable(r_num, TYPE_I32, NULL, 0));
    return code;
  } else if (node->kind == ND_RETURN) {
    // returnの場合は値を計算して返す
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* val = get_last_variable(stack);
    char* val_type = get_variable_type_str(val);
    int val_size = get_variable_size(val);
    int r_result_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_result_val,
              val_type, val_type, val->reg, val_size);
    push_code(code, "  ret %s %%%d\n", val_type, r_result_val);
    // なぜかよくわからないけどレジスタを1個空けると上手く行く
    r_register(rctx);
    return code;
  } else if (node->kind == ND_LVAR) {
    // ローカル変数の場合はその値をスタックに積む
    Variable* var = locals_r[node->offset];
    if (var->type == TYPE_ARRAY) {
      int r_var = r_register(rctx);
      push_code(code,
                "  %%%d = getelementptr inbounds %s, ptr %%%d, i64 0, i64 0\n",
                r_var, get_variable_type_str(var), var->reg);
      // int r_ptr = r_register(rctx);
      // char* ptr_to_type = get_variable_type_str(var->ptr_to);
      // push_code(code, "  %%%d = alloca %s, align 8\n", r_ptr, ptr_to_type);
      // push_code(code, "  store %s* %%%d, %s** %%%d, align 8\n", ptr_to_type,
      //           r_var, ptr_to_type, r_ptr);
      // push_variable(stack, new_variable(r_ptr, TYPE_PTR, var->ptr_to, 0));
      push_variable(stack, new_variable(r_var, TYPE_PTR, var->ptr_to, 0));
    } else {
      push_variable(stack, var);
    }
    return code;
  } else if (node->kind == ND_ASSIGN) {
    // 代入の場合は右辺を計算して左辺に代入する
    Variable* lvar = gen_lval(code, node->lhs, stack, locals_r, rctx);
    merge_code(code, generate_node(node->rhs, stack, locals_r, rctx));

    Variable* rhs = get_last_variable(stack);
    if (!is_same_type(lvar, rhs)) {
      error("代入の左辺値と右辺値の型が一致しません\n左辺: %s, 右辺: %s",
            get_variable_type_str(lvar), get_variable_type_str(rhs));
    }
    char* type = get_variable_type_str(rhs);
    int size = get_variable_size(rhs);
    int r_right_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_right_val, type,
              type, rhs->reg, size);
    push_code(code, "  store %s %%%d, %s* %%%d, align %d\n", type, r_right_val,
              type, lvar->reg, size);
    return code;
  } else if (node->kind == ND_IF) {
    // ifの場合は条件式を計算して条件によって分岐する
    // 条件式を計算する
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));

    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_cond_val,
              cond_type, cond_type, cond->reg, cond_size);
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
    int r_start_label = r_register(rctx);
    push_code(code, "  br label %%%d\n", r_start_label);
    push_code(code, "%d:\n", r_start_label);

    // 条件式を計算する
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_cond_val,
              cond_type, cond_type, cond->reg, cond_size);

    int r_cond_bool = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_cond_bool, cond_type,
              r_cond_val);

    Code* block = init_code();

    int r_next_label = r_register(rctx);
    push_code(block, "%d:\n", r_next_label);
    merge_code(block, generate_node(node->rhs, stack, locals_r, rctx));
    push_code(block, "  br label %%%d\n", r_start_label);

    int r_end_label = r_register(rctx);
    push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
              r_next_label, r_end_label);
    merge_code(code, block);
    push_code(code, "%d:\n", r_end_label);
    return code;
  } else if (node->kind == ND_FOR) {
    // forの場合は初期化式、条件式、ループ内式の順に実行する
    // 初期化式
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));

    int r_start_label = r_register(rctx);
    push_code(code, "  br label %%%d\n", r_start_label);
    push_code(code, "%d:\n", r_start_label);

    // 条件式を計算する
    merge_code(code, generate_node(node->rhs, stack, locals_r, rctx));
    Variable* cond = pop_variable(stack);
    char* cond_type = get_variable_type_str(cond);
    int cond_size = get_variable_size(cond);
    int r_cond_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_cond_val,
              cond_type, cond_type, cond->reg, cond_size);

    int r_cond_bool = r_register(rctx);
    push_code(code, "  %%%d = icmp ne %s %%%d, 0\n", r_cond_bool, cond_type,
              r_cond_val);

    Code* block = init_code();

    int r_next_label = r_register(rctx);
    push_code(block, "%d:\n", r_next_label);
    merge_code(block, generate_node(node->extra2, stack, locals_r, rctx));
    merge_code(block, generate_node(node->extra, stack, locals_r, rctx));
    push_code(block, "  br label %%%d\n", r_start_label);

    int r_end_label = r_register(rctx);
    push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
              r_next_label, r_end_label);
    merge_code(code, block);
    push_code(code, "%d:\n", r_end_label);
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
    for (int i = 0; i < node->call->args->size; i++) {
      Node* arg = vec_at(node->call->args, i);
      merge_code(code, generate_node(arg, stack, locals_r, rctx));
    }
    Variable args[node->call->args->size];
    for (int i = 0; i < node->call->args->size; i++) {
      int j = node->call->args->size - i - 1;
      Variable* arg = pop_variable(stack);
      char* arg_type = get_variable_type_str(arg);
      int arg_size = get_variable_size(arg);
      int r_arg_val = r_register(rctx);
      push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_arg_val,
                arg_type, arg_type, arg->reg, arg_size);
      args[j].reg = r_arg_val;
      args[j].type = arg->type;
      args[j].ptr_to = arg->ptr_to;
      args[j].array_size = arg->array_size;
    }

    // 関数呼び出し本体
    int r_result_val = r_register(rctx);
    // とりあえず関数の戻り値はi32固定
    push_code(code, "  %%%d = call i32 @%.*s(", r_result_val, node->call->len,
              node->call->name);
    for (int i = 0; i < node->call->args->size; i++) {
      if (i > 0) push_code(code, ", ");
      push_code(code, "%s noundef %%%d", get_variable_type_str(&args[i]),
                args[i].reg);
    }
    push_code(code, ")\n");

    // 結果をスタックに積む
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca i32, align 4\n", r_result);
    push_code(code, "  store i32 %%%d, i32* %%%d, align 4\n", r_result_val,
              r_result);
    push_variable(stack, new_variable(r_result, TYPE_I32, NULL, 0));

    return code;
  } else if (node->kind == ND_INCR || node->kind == ND_DECR) {
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* var = pop_variable(stack);
    char* var_type = get_variable_type_str(var);
    int var_size = get_variable_size(var);
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_result, var_type,
              var_size);
    int r_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_val, var_type,
              var_type, var->reg, var_size);
    push_code(code, "  store %s %%%d, %s* %%%d, align %d\n", var_type, r_val,
              var_type, r_result, var_size);
    push_variable(stack, with_reg(var, r_result));

    NodeKind op_type = node->kind == ND_INCR ? ND_ADD : ND_SUB;
    Node* op_node = new_node(ND_ASSIGN, node->lhs,
                             new_node(op_type, node->lhs, new_node_num(1)));
    merge_code(code, generate_node(op_node, stack, locals_r, rctx));
    pop_variable(stack);
    return code;
  } else if (node->kind == ND_REF) {
    // 参照の場合は左辺値のポインタをスタックに積む
    Variable* lvar = gen_lval(code, node->lhs, stack, locals_r, rctx);
    char* lvar_type = get_variable_type_str(lvar);
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s*, align 8\n", r_result, lvar_type);
    push_code(code, "  store %s* %%%d, %s** %%%d, align 8\n", lvar_type,
              lvar->reg, lvar_type, r_result);
    push_variable(stack, new_variable(r_result, TYPE_PTR, lvar, 0));
    return code;
  } else if (node->kind == ND_DEREF) {
    // 間接参照の場合はポインタの値をスタックに積む
    merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
    Variable* rhs = pop_variable(stack);
    if (!is_pointer_like(rhs)) {
      error("間接参照演算子の右辺値がポインタではありません");
    }
    char* rhs_type = get_variable_type_str(rhs);
    int r_ptr_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align 8\n", r_ptr_val,
              rhs_type, rhs_type, rhs->reg);
    char* result_type = get_variable_type_str(rhs->ptr_to);
    int result_size = get_variable_size(rhs->ptr_to);
    int r_val = r_register(rctx);
    push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_val,
              result_type, result_type, r_ptr_val, result_size);
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_result, result_type,
              result_size);
    push_code(code, "  store %s %%%d, %s* %%%d, align %d\n", result_type, r_val,
              result_type, r_result, result_size);
    push_variable(stack, with_reg(rhs->ptr_to, r_result));
    return code;
  }

  // 演算子の場合は左右のノードを先に計算する
  merge_code(code, generate_node(node->lhs, stack, locals_r, rctx));
  merge_code(code, generate_node(node->rhs, stack, locals_r, rctx));

  // スタックから2つ取り出す
  Variable* rval = pop_variable(stack);
  Variable* lval = pop_variable(stack);
  Variable* result_val = get_calc_result_type(node->kind, lval, rval);

  char* lval_type = get_variable_type_str(lval);
  int lval_size = get_variable_size(lval);
  char* rval_type = get_variable_type_str(rval);
  int rval_size = get_variable_size(rval);
  char* val_type = get_variable_type_str(result_val);
  int val_size = get_variable_size(result_val);
  int r_left_val = r_register(rctx);
  int r_right_val = r_register(rctx);
  push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_left_val,
            lval_type, lval_type, lval->reg, lval_size);
  push_code(code, "  %%%d = load %s, %s* %%%d, align %d\n", r_right_val,
            rval_type, rval_type, rval->reg, rval_size);

  if (is_pointer_like(result_val)) {
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
      push_code(code, "  %%%d = sub i64 %%%d, %%%d\n", r_sub_result, r_left_val,
                r_right_val);
      int r_sdiv_result = r_register(rctx) - 1;
      r_result_val++;
      push_code(code, "  %%%d = sdiv exact i64 %%%d, 4\n", r_sdiv_result,
                r_left_val);
      push_code(code, "  %%%d = trunc i64 %%%d to i32\n", r_result_val,
                r_sdiv_result);
    }
    int r_result = r_register(rctx);
    push_code(code, "  %%%d = alloca %s, align %d\n", r_result, val_type,
              val_size);
    push_code(code, "  store %s %%%d, %s* %%%d, align %d\n", val_type,
              r_result_val, val_type, r_result, val_size);

    push_variable(stack, with_reg(result_val, r_result));
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
    push_code(code, "  %%%d = icmp %s %s %%%d, %%%d\n", r_middle, op, val_type,
              r_left_val, r_right_val);
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
  push_code(code, "  store %s %%%d, %s* %%%d, align %d\n", val_type,
            r_result_val, val_type, r_result, val_size);
  push_variable(stack, with_reg(result_val, r_result));

  return code;
}

void generate_func(Function* func) {
  // 初期化処理
  vector* stack = new_vector();
  rctx rctx = r_init();

  printf("define dso_local i32 @%.*s(", func->len, func->name);
  int args[func->argc];
  for (int i = 0; i < func->argc; i++) {
    if (i != 0) printf(", ");
    int r_arg = r_register(rctx);
    printf("i32 noundef %%%d", r_arg);
    args[i] = r_arg;
  }
  printf(") #0 {\n");

  r_register(rctx);

  // ローカル変数の初期化
  Variable** locals_r = calloc(func->locals->size, sizeof(Variable*));
  for (int i = 0; i < func->argc; i++) {
    LVar* var = vec_at(func->locals, i);
    char* var_type = get_variable_type_str(var->var);
    int var_size = get_variable_size(var->var);
    int r = r_register(rctx);
    printf("  ; %.*s (arg)\n", var->len, var->name);
    printf("  %%%d = alloca %s, align %d\n", r, var_type, var_size);
    printf("  store %s %%%d, %s* %%%d, align %d\n", var_type, args[i], var_type,
           r, var_size);
    locals_r[i] = with_reg(var->var, r);
  }
  for (int i = func->argc; i < func->locals->size; i++) {
    LVar* lvar = vec_at(func->locals, i);
    char* var_type = get_variable_type_str(lvar->var);
    int var_size = get_variable_size(lvar->var);
    int r = r_register(rctx);
    printf("  ; %.*s (local)\n", lvar->len, lvar->name);
    printf("  %%%d = alloca %s, align %d\n", r, var_type, var_size);
    locals_r[i] = with_reg(lvar->var, r);
  }

  // 関数本体の生成
  for (int i = 0; i < func->body->size; i++) {
    Node* node = vec_at(func->body, i);
    if (node == NULL) continue;
    printf("  ; ");
    print_node(node);
    printf("\n");
    Code* result = generate_node(node, stack, locals_r, rctx);
    printf("%s", result->code);
  }

  printf("  ret i32 0\n");

  printf("}\n");

  // 後処理
  vec_free(stack);
  r_free(rctx);
}

void generate_header() {
  printf("source_filename = \"calc.c\"\n");
  printf(
      "target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"\n");
  printf("target triple = \"x86_64-pc-linux-gnu\"\n");
  printf("\n");
}

void generate_print() {
  printf("declare i32 @print(i32 noundef, ...) #1\n");
  printf("declare i32 @scan() #0\n");
  printf(
      "declare i32 @alloc4(i32** noundef, i32 noundef, i32 noundef, i32 "
      "noundef, i32 noundef) #0\n");
}

void generate(vector* code) {
  generate_header();

  // コード生成
  for (int i = 0; i < code->size; i++) {
    Function* func = vec_at(code, i);
    generate_func(func);
  }

  printf("\n");

  generate_print();
}
