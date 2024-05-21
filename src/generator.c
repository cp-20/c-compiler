#include "generator.h"

#include <stdio.h>

#include "code.h"
#include "error.h"
#include "llvm.h"
#include "parser.h"
#include "vector.h"

// 左辺値のポインタ (レジスタ) を返す
int gen_lval(Node* node, int* locals_r) {
  if (node->kind != ND_LVAR) error("代入の左辺値が変数ではありません");

  return locals_r[node->offset];
}

Code* generate_node(Node* node, vector* stack, int* locals_r) {
  Code* code = init_code();

  if (node->kind == ND_NUM) {
    // 数字の場合はそのままスタックに積む
    int* r_num = r_register_ptr();
    push_code(code, "  %%%d = alloca i32, align 4\n", *r_num);
    push_code(code, "  store i32 %d, i32* %%%d, align 4\n", node->val, *r_num);
    vec_push_last(stack, r_num);
    return code;
  } else if (node->kind == ND_RETURN) {
    // returnの場合は値を計算して返す
    merge_code(code, generate_node(node->lhs, stack, locals_r));
    int r_result = *(int*)vec_at(stack, stack->size - 1);
    int r_result_val = r_register();
    push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_result_val,
              r_result);
    push_code(code, "  ret i32 %%%d\n", r_result_val);
    // なぜかよくわからないけどレジスタを1個空けると上手く行く
    r_register();
    return code;
  } else if (node->kind == ND_LVAR) {
    // ローカル変数の場合はその値をスタックに積む
    vec_push_last(stack, &locals_r[node->offset]);
    return code;
  } else if (node->kind == ND_ASSIGN) {
    // 代入の場合は右辺を計算して左辺に代入する
    int lptr = gen_lval(node->lhs, locals_r);
    merge_code(code, generate_node(node->rhs, stack, locals_r));

    int r_right = *(int*)vec_at(stack, stack->size - 1);
    int r_right_val = r_register();
    push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_right_val,
              r_right);
    push_code(code, "  store i32 %%%d, i32* %%%d, align 4\n", r_right_val,
              lptr);
    return code;
  } else if (node->kind == ND_IF) {
    // ifの場合は条件式を計算して条件によって分岐する
    // 条件式を計算する
    merge_code(code, generate_node(node->lhs, stack, locals_r));

    int r_cond = *(int*)vec_at(stack, stack->size - 1);
    int r_cond_val = r_register();
    push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_cond_val,
              r_cond);
    int r_cond_bool = r_register();
    push_code(code, "  %%%d = icmp ne i32 %%%d, 0\n", r_cond_bool, r_cond_val);

    int r_then_label = r_register();

    Code* block = init_code();
    push_code(block, "%d:\n", r_then_label);
    merge_code(block, generate_node(node->rhs, stack, locals_r));

    int r_next_label = r_register();

    if (node->extra != NULL) {
      Code* sub_block = init_code();
      push_code(sub_block, "%d:\n", r_next_label);
      merge_code(sub_block, generate_node(node->extra, stack, locals_r));
      int r_final_label = r_register();
      push_code(sub_block, "  br label %%%d\n", r_final_label);

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
    int r_start_label = r_register();
    push_code(code, "  br label %%%d\n", r_start_label);
    push_code(code, "%d:\n", r_start_label);

    // 条件式を計算する
    merge_code(code, generate_node(node->lhs, stack, locals_r));
    int r_cond = *(int*)vec_at(stack, stack->size - 1);
    int r_cond_val = r_register();
    push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_cond_val,
              r_cond);

    int r_cond_bool = r_register();
    push_code(code, "  %%%d = icmp ne i32 %%%d, 0\n", r_cond_bool, r_cond_val);

    Code* block = init_code();

    int r_next_label = r_register();
    push_code(block, "%d:\n", r_next_label);
    merge_code(block, generate_node(node->rhs, stack, locals_r));
    push_code(block, "  br label %%%d\n", r_start_label);

    int r_end_label = r_register();
    push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
              r_next_label, r_end_label);
    merge_code(code, block);
    push_code(code, "%d:\n", r_end_label);
    return code;
  } else if (node->kind == ND_FOR) {
    // forの場合は初期化式、条件式、ループ内式の順に実行する
    // 初期化式
    merge_code(code, generate_node(node->lhs, stack, locals_r));

    int r_start_label = r_register();
    push_code(code, "  br label %%%d\n", r_start_label);
    push_code(code, "%d:\n", r_start_label);

    // 条件式を計算する
    merge_code(code, generate_node(node->rhs, stack, locals_r));
    int r_cond = *(int*)vec_at(stack, stack->size - 1);
    int r_cond_val = r_register();
    push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_cond_val,
              r_cond);

    int r_cond_bool = r_register();
    push_code(code, "  %%%d = icmp ne i32 %%%d, 0\n", r_cond_bool, r_cond_val);

    Code* block = init_code();

    int r_next_label = r_register();
    push_code(block, "%d:\n", r_next_label);
    merge_code(block, generate_node(node->extra2, stack, locals_r));
    merge_code(block, generate_node(node->extra, stack, locals_r));
    push_code(block, "  br label %%%d\n", r_start_label);

    int r_end_label = r_register();
    push_code(code, "  br i1 %%%d, label %%%d, label %%%d\n", r_cond_bool,
              r_next_label, r_end_label);
    merge_code(code, block);
    push_code(code, "%d:\n", r_end_label);
    return code;
  } else if (node->kind == ND_BLOCK) {
    // ブロックの場合は各文を順に実行する
    for (int i = 0; i < node->stmts->size; i++) {
      merge_code(code, generate_node(vec_at(node->stmts, i), stack, locals_r));
    }
    return code;
  }

  // 演算子の場合は左右のノードを先に計算する
  merge_code(code, generate_node(node->lhs, stack, locals_r));
  merge_code(code, generate_node(node->rhs, stack, locals_r));

  // スタックから2つ取り出す
  int r_right = *(int*)vec_pop(stack);
  int r_left = *(int*)vec_pop(stack);
  int r_left_val = r_register();
  int r_right_val = r_register();
  push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_left_val,
            r_left);
  push_code(code, "  %%%d = load i32, i32* %%%d, align 4\n", r_right_val,
            r_right);

  // 計算する
  int r_result_val = r_register();
  if (node->kind == ND_ADD || node->kind == ND_SUB || node->kind == ND_MUL) {
    char op[4];
    if (node->kind == ND_ADD) {
      sprintf(op, "add");
    } else if (node->kind == ND_SUB) {
      sprintf(op, "sub");
    } else if (node->kind == ND_MUL) {
      sprintf(op, "mul");
    }
    push_code(code, "  %%%d = %s nsw i32 %%%d, %%%d\n", r_result_val, op,
              r_left_val, r_right_val);
  } else if (node->kind == ND_DIV) {
    push_code(code, "  %%%d = sdiv i32 %%%d, %%%d\n", r_result_val, r_left_val,
              r_right_val);
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
    int r_middle = r_register() - 1;
    r_result_val++;
    push_code(code, "  %%%d = icmp %s i32 %%%d, %%%d\n", r_middle, op,
              r_left_val, r_right_val);
    push_code(code, "  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
  }

  // 結果を再びスタックに積む
  int* r_result = r_register_ptr();
  push_code(code, "  %%%d = alloca i32, align 4\n", *r_result);
  push_code(code, "  store i32 %%%d, i32* %%%d, align 4\n", r_result_val,
            *r_result);
  vec_push_last(stack, r_result);

  return code;
}

void generate_header() {
  printf("source_filename = \"calc.c\"\n");
  printf(
      "target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"\n");
  printf("target triple = \"x86_64-pc-linux-gnu\"\n");
  printf("\n");

  // 最後の出力用の文字列
  printf(
      "@.str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", "
      "align 1\n");
  printf("\n");
}

void generate_printf() {
  printf("\n");
  printf("declare i32 @printf(ptr noundef, ...) #1");
}

void generate(vector* code) {
  generate_header();

  // 初期化処理
  vector* stack = new_vector();

  int local_count = 0;
  for (LVar* var = locals; var; var = var->next) local_count++;
  int* locals_r = malloc(sizeof(int) * local_count);

  // main2関数のはじまり
  printf("define dso_local i32 @main2() #0 {\n");

  for (LVar* var = locals; var; var = var->next) {
    int r = r_register();
    printf("  ; %.*s (local var)\n", var->len, var->name);
    printf("  %%%d = alloca i32, align 4\n", r);
    locals_r[var->offset] = r;
  }

  // コード生成
  for (int i = 0; i < code->size; i++) {
    printf("  ; ");
    print_node(vec_at(code, i));
    printf("\n");
    Code* result = generate_node(vec_at(code, i), stack, locals_r);
    printf("%s", result->code);
  }

  // スタックトップを返す
  int r_result = *(int*)vec_at(stack, stack->size - 1);
  int r_result_val = r_register();
  printf("  %%%d = load i32, i32* %%%d, align 4\n", r_result_val, r_result);
  printf("  ret i32 %%%d\n", r_result_val);
  r_register();

  // main2関数の終わり
  printf("}\n");

  printf("\n");

  // main関数のはじまり
  printf("define dso_local i32 @main() #0 {\n");

  // main2関数を呼び出す
  int r_result_main = r_register();
  printf("  %%%d = call i32 @main2()\n", r_result_main);

  // 結果を出力
  int r_print_result = r_register();
  printf(
      "  %%%d = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef "
      "%%%d)\n",
      r_print_result, r_result_main);

  // main関数の終わり
  printf("  ret i32 0\n");
  printf("}\n");

  generate_printf();
}