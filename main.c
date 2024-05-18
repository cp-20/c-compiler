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

  // コード生成
  generate(code);

  return 0;
}