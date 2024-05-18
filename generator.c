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

bool returned = false;

int generate_node(Node* node, vector* stack, int* locals_r) {
  switch (node->kind) {
      // 数字の場合はそのままスタックに積む
    case ND_NUM: {
      int* r_num = malloc(sizeof(int));
      *r_num = r_register();
      printf("  %%%d = alloca i32, align 4\n", *r_num);
      printf("  store i32 %d, i32* %%%d, align 4\n", node->val, *r_num);
      vec_push_last(stack, r_num);
      return *r_num;
    } break;

    // returnの場合は値を計算して返す
    case ND_RETURN: {
      printf("  ; return\n");
      generate_node(node->lhs, stack, locals_r);
      int r_result = *(int*)vec_at(stack, stack->size - 1);
      returned = true;
      return r_result;
    } break;

    // ローカル変数の場合はその値をスタックに積む
    case ND_LVAR: {
      vec_push_last(stack, &locals_r[node->offset]);
      return locals_r[node->offset];
    } break;

    // 代入の場合は右辺を計算して左辺に代入する
    case ND_ASSIGN: {
      int lptr = gen_lval(node->lhs, locals_r);
      generate_node(node->rhs, stack, locals_r);

      int r_right = *(int*)vec_at(stack, stack->size - 1);
      int r_right_val = r_register();
      printf("  %%%d = load i32, i32* %%%d, align 4\n", r_right_val, r_right);
      printf("  store i32 %%%d, i32* %%%d, align 4\n", r_right_val, lptr);
      return r_right;
    } break;
  }

  // 演算子の場合は左右のノードを先に計算する
  generate_node(node->lhs, stack, locals_r);
  generate_node(node->rhs, stack, locals_r);

  // スタックから2つ取り出す
  int r_right = *(int*)vec_pop(stack);
  int r_left = *(int*)vec_pop(stack);
  int r_left_val = r_register();
  int r_right_val = r_register();
  printf("  %%%d = load i32, i32* %%%d, align 4\n", r_left_val, r_left);
  printf("  %%%d = load i32, i32* %%%d, align 4\n", r_right_val, r_right);

  // 計算する
  int r_result_val = r_register();
  switch (node->kind) {
    case ND_ADD: {
      printf("  %%%d = add nsw i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
    } break;
    case ND_SUB: {
      printf("  %%%d = sub nsw i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
    } break;
    case ND_MUL: {
      printf("  %%%d = mul nsw i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
    } break;
    case ND_DIV: {
      printf("  %%%d = sdiv i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
    } break;
    case ND_EQ: {
      int r_middle = r_register() - 1;
      r_result_val++;
      printf("  %%%d = icmp eq i32 %%%d, %%%d\n", r_middle, r_left_val,
             r_right_val);
      printf("  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
    } break;
    case ND_NE: {
      int r_middle = r_register() - 1;
      r_result_val++;
      printf("  %%%d = icmp ne i32 %%%d, %%%d\n", r_middle, r_left_val,
             r_right_val);
      printf("  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
    } break;
    case ND_LT: {
      int r_middle = r_register() - 1;
      r_result_val++;
      printf("  %%%d = icmp slt i32 %%%d, %%%d\n", r_middle, r_left_val,
             r_right_val);
      printf("  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
    } break;
    case ND_LE: {
      int r_middle = r_register() - 1;
      r_result_val++;
      printf("  %%%d = icmp sle i32 %%%d, %%%d\n", r_middle, r_left_val,
             r_right_val);
      printf("  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
    } break;
    case ND_GT: {
      int r_middle = r_register() - 1;
      r_result_val++;
      printf("  %%%d = icmp sgt i32 %%%d, %%%d\n", r_middle, r_left_val,
             r_right_val);
      printf("  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
    } break;
    case ND_GE: {
      int r_middle = r_register() - 1;
      r_result_val++;
      printf("  %%%d = icmp sge i32 %%%d, %%%d\n", r_middle, r_left_val,
             r_right_val);
      printf("  %%%d = zext i1 %%%d to i32\n", r_result_val, r_middle);
    } break;
  }

  // 結果を再びスタックに積む
  int r_result = r_register();
  printf("  %%%d = alloca i32, align 4\n", r_result);
  printf("  store i32 %%%d, i32* %%%d, align 4\n", r_result_val, r_result);
  vec_push_last(stack, &r_result);

  // スタックの最後を返す
  return r_result;
}

int generate(vector* code) {
  // 初期化処理
  vector* stack = new_vector();

  int local_count = 0;
  for (LVar* var = locals; var; var = var->next) local_count++;
  int* locals_r = malloc(sizeof(int) * local_count);

  for (LVar* var = locals; var; var = var->next) {
    int r = r_register();
    printf("  ; %.*s (local var)\n", var->len, var->name);
    printf("  %%%d = alloca i32, align 4\n", r);
    locals_r[var->offset] = r;
  }

  int r_result;
  // コード生成
  for (int i = 0; i < code->size; i++) {
    printf("  ; ");
    print_node(vec_at(code, i));
    printf("\n");
    r_result = generate_node(vec_at(code, i), stack, locals_r);
    if (returned) break;
  }

  return r_result;
}
