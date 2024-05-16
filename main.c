#include <stdio.h>

#include "error.h"
#include "generator.h"
#include "llvm.h"
#include "parser.h"
#include "tokenizer.h"

int main(int argc, char** argv) {
  char* input = argv[1];

  if (argc != 2) {
    error("引数の個数が正しくありません");
    return 1;
  }

  // エラー出力の初期化
  init_error(input);

  // トークナイズしてパースする
  Token* token = tokenize(input);

  vector* code = parse(token);

  // header
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

  // main関数のはじまり
  printf("  define dso_local i32 @main() #0 {\n");

  // コード生成
  int r_result = generate(code);

  // 結果の出力
  int r_result_val = r_register();
  printf("  %%%d = load i32, i32* %%%d, align 4\n", r_result_val, r_result);
  int r_print_result = r_register();
  printf(
      "  %%%d = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef "
      "%%%d)\n",
      r_print_result, r_result_val);

  // main関数の終わり
  printf("  ret i32 0\n");
  printf("}\n");

  // printf関数の宣言
  printf("\n");
  printf("declare i32 @printf(ptr noundef, ...) #1");

  return 0;
}