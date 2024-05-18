#include "generator.h"

#include <stdio.h>

#include "error.h"
#include "llvm.h"
#include "parser.h"
#include "vector.h"

// 左辺値のポインタ (レジスタ) を返す
int gen_lval(Node* node, int* locals_r) {
  if (node->kind != ND_LVAR) error("代入の左辺値が変数ではありません");

  return locals_r[node->offset];
}

char* generate_node(Node* node, vector* stack, int* locals_r) {
  char* code = malloc(65536);
  int code_len = 0;

  switch (node->kind) {
    // 数字の場合はそのままスタックに積む
    case ND_NUM: {
      int* r_num = malloc(sizeof(int));
      *r_num = r_register();
      code_len +=
          sprintf(code + code_len, "  %%%d = alloca i32, align 4\n", *r_num);
      code_len +=
          sprintf(code + code_len, "  store i32 %d, i32* %%%d, align 4\n",
                  node->val, *r_num);
      vec_push_last(stack, r_num);
      return code;
    } break;

    // returnの場合は値を計算して返す
    case ND_RETURN: {
      code_len += sprintf(code + code_len, "%s",
                          generate_node(node->lhs, stack, locals_r));
      int r_result = *(int*)vec_at(stack, stack->size - 1);
      int r_result_val = r_register();
      code_len +=
          sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
                  r_result_val, r_result);
      code_len += sprintf(code + code_len, "  ret i32 %%%d\n", r_result_val);
      // なぜかよくわからないけどレジスタを1個空けると上手く行く
      r_register();
      return code;
    } break;

    // ローカル変数の場合はその値をスタックに積む
    case ND_LVAR: {
      vec_push_last(stack, &locals_r[node->offset]);
      return code;
    } break;

    // 代入の場合は右辺を計算して左辺に代入する
    case ND_ASSIGN: {
      int lptr = gen_lval(node->lhs, locals_r);
      code_len += sprintf(code + code_len, "%s",
                          generate_node(node->rhs, stack, locals_r));

      int r_right = *(int*)vec_at(stack, stack->size - 1);
      int r_right_val = r_register();
      code_len +=
          sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
                  r_right_val, r_right);
      code_len +=
          sprintf(code + code_len, "  store i32 %%%d, i32* %%%d, align 4\n",
                  r_right_val, lptr);
      return code;
    } break;

    // ifの場合は条件式を計算して条件によって分岐する
    case ND_IF: {
      // 条件式を計算する
      code_len += sprintf(code + code_len, "%s",
                          generate_node(node->lhs, stack, locals_r));

      int r_cond = *(int*)vec_at(stack, stack->size - 1);
      int r_cond_val = r_register();
      code_len +=
          sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
                  r_cond_val, r_cond);

      int r_cond_bool = r_register();
      code_len += sprintf(code + code_len, "  %%%d = icmp ne i32 %%%d, 0\n",
                          r_cond_bool, r_cond_val);

      int r_then_label = r_register();

      char block[65536];
      int block_len = 0;
      block_len += sprintf(block + block_len, "%d:\n", r_then_label);
      block_len += sprintf(block + block_len, "%s",
                           generate_node(node->rhs, stack, locals_r));

      int r_next_label = r_register();

      if (node->extra != NULL) {
        char sub_block[65536];
        int sub_block_len = 0;
        sub_block_len +=
            sprintf(sub_block + sub_block_len, "%d:\n", r_next_label);
        sub_block_len += sprintf(sub_block + sub_block_len, "%s",
                                 generate_node(node->extra, stack, locals_r));
        int r_final_label = r_register();
        block_len +=
            sprintf(block + block_len, "  br label %%%d\n", r_final_label);
        block_len += sprintf(block + block_len, "%s", sub_block);
        block_len +=
            sprintf(block + block_len, "  br label %%%d\n", r_final_label);
        block_len += sprintf(block + block_len, "%d:\n", r_final_label);
      } else {
        block_len +=
            sprintf(block + block_len, "  br label %%%d\n", r_next_label);
        block_len += sprintf(block + block_len, "%d:\n", r_next_label);
      }

      code_len +=
          sprintf(code + code_len, "  br i1 %%%d, label %%%d, label %%%d\n",
                  r_cond_bool, r_then_label, r_next_label);
      code_len += sprintf(code + code_len, "%s", block);
      return code;
    } break;

    // whileの場合は条件式を計算して条件によってループする
    case ND_WHILE: {
      int r_start_label = r_register();
      code_len += sprintf(code + code_len, "  br label %%%d\n", r_start_label);
      code_len += sprintf(code + code_len, "%d:\n", r_start_label);

      // 条件式を計算する
      code_len += sprintf(code + code_len, "%s",
                          generate_node(node->lhs, stack, locals_r));
      int r_cond = *(int*)vec_at(stack, stack->size - 1);
      int r_cond_val = r_register();
      code_len +=
          sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
                  r_cond_val, r_cond);

      int r_cond_bool = r_register();
      code_len += sprintf(code + code_len, "  %%%d = icmp ne i32 %%%d, 0\n",
                          r_cond_bool, r_cond_val);

      char block[65536];
      int block_len = 0;

      int r_next_label = r_register();
      block_len += sprintf(block + block_len, "%d:\n", r_next_label);
      block_len += sprintf(block + block_len, "%s",
                           generate_node(node->rhs, stack, locals_r));
      block_len +=
          sprintf(block + block_len, "  br label %%%d\n", r_start_label);

      int r_end_label = r_register();
      code_len +=
          sprintf(code + code_len, "  br i1 %%%d, label %%%d, label %%%d\n",
                  r_cond_bool, r_next_label, r_end_label);
      code_len += sprintf(code + code_len, "%s", block);
      code_len += sprintf(code + code_len, "%d:\n", r_end_label);
      return code;
    } break;

    case ND_FOR: {
      // 初期化式
      code_len += sprintf(code + code_len, "%s",
                          generate_node(node->lhs, stack, locals_r));

      int r_start_label = r_register();
      code_len += sprintf(code + code_len, "  br label %%%d\n", r_start_label);
      code_len += sprintf(code + code_len, "%d:\n", r_start_label);

      // 条件式を計算する
      code_len += sprintf(code + code_len, "%s",
                          generate_node(node->rhs, stack, locals_r));
      int r_cond = *(int*)vec_at(stack, stack->size - 1);
      int r_cond_val = r_register();
      code_len +=
          sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
                  r_cond_val, r_cond);

      int r_cond_bool = r_register();
      code_len += sprintf(code + code_len, "  %%%d = icmp ne i32 %%%d, 0\n",
                          r_cond_bool, r_cond_val);

      char block[65536];
      int block_len = 0;

      int r_next_label = r_register();
      block_len += sprintf(block + block_len, "%d:\n", r_next_label);
      block_len += sprintf(block + block_len, "%s",
                           generate_node(node->extra2, stack, locals_r));
      block_len += sprintf(block + block_len, "%s",
                           generate_node(node->extra, stack, locals_r));
      block_len +=
          sprintf(block + block_len, "  br label %%%d\n", r_start_label);

      int r_end_label = r_register();
      code_len +=
          sprintf(code + code_len, "  br i1 %%%d, label %%%d, label %%%d\n",
                  r_cond_bool, r_next_label, r_end_label);
      code_len += sprintf(code + code_len, "%s", block);
      code_len += sprintf(code + code_len, "%d:\n", r_end_label);
      return code;
    } break;
  }

  // 演算子の場合は左右のノードを先に計算する
  code_len +=
      sprintf(code + code_len, "%s", generate_node(node->lhs, stack, locals_r));
  code_len +=
      sprintf(code + code_len, "%s", generate_node(node->rhs, stack, locals_r));

  // スタックから2つ取り出す
  int r_right = *(int*)vec_pop(stack);
  int r_left = *(int*)vec_pop(stack);
  int r_left_val = r_register();
  int r_right_val = r_register();
  code_len +=
      sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
              r_left_val, r_left);
  code_len +=
      sprintf(code + code_len, "  %%%d = load i32, i32* %%%d, align 4\n",
              r_right_val, r_right);

  // 計算する
  int r_result_val = r_register();
  switch (node->kind) {
    case ND_ADD: {
      code_len += sprintf(code + code_len, "  %%%d = add nsw i32 %%%d, %%%d\n",
                          r_result_val, r_left_val, r_right_val);
    } break;
    case ND_SUB: {
      code_len += sprintf(code + code_len, "  %%%d = sub nsw i32 %%%d, %%%d\n",
                          r_result_val, r_left_val, r_right_val);
    } break;
    case ND_MUL: {
      code_len += sprintf(code + code_len, "  %%%d = mul nsw i32 %%%d, %%%d\n",
                          r_result_val, r_left_val, r_right_val);
    } break;
    case ND_DIV: {
      code_len += sprintf(code + code_len, "  %%%d = sdiv i32 %%%d, %%%d\n",
                          r_result_val, r_left_val, r_right_val);
    } break;
    case ND_EQ: {
      int r_middle = r_register() - 1;
      r_result_val++;
      code_len += sprintf(code + code_len, "  %%%d = icmp eq i32 %%%d, %%%d\n",
                          r_middle, r_left_val, r_right_val);
      code_len += sprintf(code + code_len, "  %%%d = zext i1 %%%d to i32\n",
                          r_result_val, r_middle);
    } break;
    case ND_NE: {
      int r_middle = r_register() - 1;
      r_result_val++;
      code_len += sprintf(code + code_len, "  %%%d = icmp ne i32 %%%d, %%%d\n",
                          r_middle, r_left_val, r_right_val);
      code_len += sprintf(code + code_len, "  %%%d = zext i1 %%%d to i32\n",
                          r_result_val, r_middle);
    } break;
    case ND_LT: {
      int r_middle = r_register() - 1;
      r_result_val++;
      code_len += sprintf(code + code_len, "  %%%d = icmp slt i32 %%%d, %%%d\n",
                          r_middle, r_left_val, r_right_val);
      code_len += sprintf(code + code_len, "  %%%d = zext i1 %%%d to i32\n",
                          r_result_val, r_middle);
    } break;
    case ND_LE: {
      int r_middle = r_register() - 1;
      r_result_val++;
      code_len += sprintf(code + code_len, "  %%%d = icmp sle i32 %%%d, %%%d\n",
                          r_middle, r_left_val, r_right_val);
      code_len += sprintf(code + code_len, "  %%%d = zext i1 %%%d to i32\n",
                          r_result_val, r_middle);
    } break;
    case ND_GT: {
      int r_middle = r_register() - 1;
      r_result_val++;
      code_len += sprintf(code + code_len, "  %%%d = icmp sgt i32 %%%d, %%%d\n",
                          r_middle, r_left_val, r_right_val);
      code_len += sprintf(code + code_len, "  %%%d = zext i1 %%%d to i32\n",
                          r_result_val, r_middle);
    } break;
    case ND_GE: {
      int r_middle = r_register() - 1;
      r_result_val++;
      code_len += sprintf(code + code_len, "  %%%d = icmp sge i32 %%%d, %%%d\n",
                          r_middle, r_left_val, r_right_val);
      code_len += sprintf(code + code_len, "  %%%d = zext i1 %%%d to i32\n",
                          r_result_val, r_middle);
    } break;
  }

  // 結果を再びスタックに積む
  int r_result = r_register();
  code_len +=
      sprintf(code + code_len, "  %%%d = alloca i32, align 4\n", r_result);
  code_len += sprintf(code + code_len, "  store i32 %%%d, i32* %%%d, align 4\n",
                      r_result_val, r_result);
  vec_push_last(stack, &r_result);

  return code;
}

void generate_header() {
  printf("source_filename = \"calc.c\"\n");
  printf(
      "target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"\n");
  printf("target triple = \"x86_64-unknown-linux-gnu\"\n");
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
    printf("%s", generate_node(vec_at(code, i), stack, locals_r));
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
