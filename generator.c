#include <stdio.h>

#include "llvm.h"
#include "parser.h"
#include "vector.h"

int generate(Node* node, vector* stack) {
  if (stack == NULL) {
    stack = new_vector();
  }

  // 数字の場合はそのままスタックに積む
  if (node->kind == ND_NUM) {
    int* r_num = malloc(sizeof(int));
    *r_num = r_register();
    printf("  %%%d = alloca i32, align 4\n", *r_num);
    printf("  store i32 %d, i32* %%%d, align 4\n", node->val, *r_num);
    vec_push_last(stack, r_num);
    return *r_num;
  }

  // 演算子の場合は左右のノードを先に計算する
  generate(node->lhs, stack);
  generate(node->rhs, stack);

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
    case ND_ADD:
      printf("  %%%d = add nsw i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
      break;
    case ND_SUB:
      printf("  %%%d = sub nsw i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
      break;
    case ND_MUL:
      printf("  %%%d = mul nsw i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
      break;
    case ND_DIV:
      printf("  %%%d = sdiv i32 %%%d, %%%d\n", r_result_val, r_left_val,
             r_right_val);
      break;
  }

  // 結果を再びスタックに積む
  int r_result = r_register();
  printf("  %%%d = alloca i32, align 4\n", r_result);
  printf("  store i32 %%%d, i32* %%%d, align 4\n", r_result_val, r_result);
  vec_push_last(stack, &r_result);

  // スタックの最後を返す
  return r_result;
}