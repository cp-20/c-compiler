#include "preprocessor.h"

#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "debug.h"
#include "error.h"
#include "seek.h"
#include "tokenizer.h"
#include "vector.h"

char* dirname(char* path) {
  char* p = path + strlen(path) - 1;
  while (p >= path && *p != '/') p--;
  if (p < path) return ".";
  char* output = malloc(p - path + 1);
  snprintf(output, p - path + 1, "%s", path);
  return output;
}

char* join_path(char* dir, char* file) {
  char* path = malloc(strlen(dir) + strlen(file) + 2);
  snprintf(path, strlen(dir) + strlen(file) + 2, "%s/%s", dir, file);
  return path;
}

char* process_include(char* input, vector* included_files) {
  char* p = input + 9;
  while (*p == ' ') p++;
  if (*p != '<' && *p != '"') {
    error_at(p, "不正な include ディレクティブです: %.*s", 1, p);
    return NULL;
  }

  bool is_system_include = *p == '<';

  p++;
  char* q = p;
  while (*q && *q != '>' && *q != '"') q++;
  if (!*q) {
    error_at(q, "閉じられていない include ディレクティブです");
    return NULL;
  }

  char* including_file = malloc(q - p + 1);
  snprintf(including_file, q - p + 1, "%s", p);

  // システムファイルを真面目にincludeするのは面倒なので諦める
  if (is_system_include) return q + 1;

  // 自動で2重includeを防止 (include guard)
  for (int i = 0; i < included_files->size; i++) {
    char* file = vec_at(included_files, i);
    if (strcmp(file, including_file) == 0) {
      return q + 1;
    }
  }
  vec_push_last(included_files, including_file);

  // 相対ファイルパスに変換
  char* current_dir = dirname(filename);
  char* file = join_path(current_dir, including_file);

  char* included = read_file(file);
  if (!included) {
    error("File not found: %s", including_file);
    return NULL;
  }

  char* rest = q + 1;
  char* new_output = calloc(strlen(included) + strlen(rest) + 1, 1);
  strcat(new_output, included);
  strcat(new_output, rest);
  return new_output;
}

char* process_define(char* input) {
  char* p = input + 8;
  while (*p == ' ') p++;
  if (!is_alpha(*p)) {
    error_at(p, "不正な define ディレクティブです: %.*s", 1, p);
    return NULL;
  }

  char* q = p + 1;
  while (is_alpha(*q) || is_num(*q)) q++;
  int len = q - p;
  char* name = malloc(len + 1);
  snprintf(name, len + 1, "%s", p);
  print_debug(COL_BLUE "[preprocessor] " COL_GREEN "[#define] " COL_RESET
                       "name = %s",
              name);

  // 引数付きマクロはサポートしない
  if (*q == '(') {
    error_at(q, "引数付きマクロはサポートされていません");
    return NULL;
  }

  while (*q == ' ') q++;
  if (*q == '\n') {
    free(name);
    return q;
  }

  char* r = q;
  while (*r != '\n') r++;
  char* body = malloc(r - q + 1);
  snprintf(body, r - q + 1, "%s", q);

  char* rest = r++;
  char* new_output = calloc(strlen(rest) + 1, 1);
  while (*r) {
    // コメントはスキップ
    if (strncmp(r, "//", 2) == 0) {
      while (*r != '\n') r++;
    }
    if (strncmp(r, "/*", 2) == 0) {
      while (strncmp(r, "*/", 2) != 0) r++;
    }

    // 文字列中はスキップ
    if (*r == '"') {
      r++;
      while (*r != '"') r++;
    }
    if (*r == '\'') {
      r++;
      while (*r != '\'') r++;
    }

    if (strncmp(r, name, len) == 0) {
      strncat(new_output, rest, r - rest);
      strcat(new_output, body);
      r += len;
      rest = r;
    }
    r++;
  }
  strcat(new_output, rest);
  return new_output;
}

char* preprocess(char* input) {
  vector* included_files = new_vector();
  char* output = calloc(strlen(input) + 1, 1);

  print_debug(COL_BLUE "[preprocessor] " COL_RESET "Start preprocessing...");

  char* p = input;
  char* q = p;
  while (*p) {
    if (strncmp(p, "#include ", 9) == 0) {
      print_debug(COL_BLUE "[preprocessor] " COL_RESET
                           "Found #include directive");
      strncat(output, q, p - q);
      p = process_include(p, included_files);
      q = p;
    };

    if (strncmp(p, "#pragma ", 8) == 0) {
      print_debug(COL_BLUE "[preprocessor] " COL_RESET
                           "Found #pragma directive");
      strncat(output, q, p - q);
      while (*p != '\n') p++;
      q = p;
    }

    if (strncmp(p, "#define ", 8) == 0) {
      print_debug(COL_BLUE "[preprocessor] " COL_RESET
                           "Found #define directive");
      strncat(output, q, p - q);
      p = process_define(p);
      q = p;
    }

    // 次の行まで飛ばす
    while (*p != '\n') p++;
    p++;
  }

  strncat(output, q, p - q);

  print_debug(COL_BLUE "[preprocessor] " COL_RESET "Preprocessing done");

  return output;
}

// うまく処理する (char*のインスタンスの扱い)
