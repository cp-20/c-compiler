#include <stdbool.h>
#include <stdio.h>

#include "code.h"
#include "debug.h"
#include "error.h"
#include "generator.h"
#include "parser.h"
#include "preprocessor.h"
#include "seek.h"
#include "tokenizer.h"

int main(int argc, char** argv) {
  if (argc == 1) {
    error("引数の個数が正しくありません");
    return 1;
  }

  f_debug = false;
  char* output_file = NULL;
  for (int i = 1; i < argc; i++) {
    char* option = argv[i];
    if (memcmp(option, "--debug", 7) == 0) {
      f_debug = true;
    } else if (memcmp(option, "--output", 8) == 0) {
      output_file = argv[++i];
    } else {
      filename = option;
    }
  }

  if (filename == NULL) {
    error("ファイル名が指定されていません");
    return 1;
  }

  if (output_file == NULL) {
    output_file = "a.out";
  }

  // ファイルを読み込む
  char* input = read_file(filename);

  // エラー出力の初期化
  init_error(input);

  input = preprocess(input);
  Token* token = tokenize(input);
  Program* code = parse(token);
  Code* result = generate(code);
  char* result_str = result->code;
  result_str[result->size] = '\0';

  write_file(output_file, result_str);

  return 0;
}
