#include <stdbool.h>
#include <stdio.h>

#include "debug.h"
#include "error.h"
#include "generator.h"
#include "llvm.h"
#include "parser.h"
#include "tokenizer.h"

int main(int argc, char** argv) {
  if (argc == 1) {
    error("引数の個数が正しくありません");
    return 1;
  }

  char* input;
  for (int i = 1; i < argc; i++) {
    input = argv[i];
    if (memcmp(input, "--debug", 7) == 0) {
      f_debug = true;
    }
  }

  // エラー出力の初期化
  init_error(input);

  // トークナイズしてパースする
  print_debug("START tokenize");
  Token* token = tokenize(input);
  print_debug("END tokenize");

  print_debug("START parse");
  Program* code = parse(token);
  print_debug("END parse");

  // コード生成
  print_debug("START generate");
  generate(code);
  print_debug("END generate");

  return 0;
}
