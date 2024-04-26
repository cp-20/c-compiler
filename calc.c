#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  TK_RESERVED,  // 記号
  TK_NUM,       // 整数
  TK_EOF,       // 入力の終わり
} TokenKind;

typedef struct Token {
  TokenKind kind;
  struct Token *next;
  int val;
  char *str;
  int len;
} Token;

Token *token;

Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

bool is_space(char c) {
  if (c == ' ') return true;
  if (c == '\t') return true;
  if (c == '\n') return true;
  if (c == '\r') return true;
  return false;
}

bool is_digit(char c) { return '0' <= c && c <= '9'; }

void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) return false;
  token = token->next;
  return true;
}

void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error("'%c'ではありません", op);
  token = token->next;
}

int expect_number() {
  if (token->kind != TK_NUM) error("数ではありません");
  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // 空白文字をスキップ
    if (is_space(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (is_digit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error("トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int r_register() {
  static int i = 1;
  return i++;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    error("引数の個数が正しくありません");
    return 1;
  }

  // トークナイズする
  token = tokenize(argv[1]);

  printf("source_filename = \"calc.c\"\n");
  printf(
      "target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"\n");
  printf("target triple = \"x86_64-unknown-linux-gnu\"\n");
  printf("\n");

  printf(
      "  @.str = private unnamed_addr constant [4 x i8] c\"%%d\\0A\\00\", "
      "align 1\n");
  printf("\n");

  printf("  define dso_local i32 @main() #0 {\n");

  int number = expect_number();
  int r_main = r_register();
  printf("  %%%d = alloca i32, align 4\n", r_main);
  printf("  store i32 %d, i32* %%%d, align 4\n", number, r_main);

  // `+ <数>`あるいは`- <数>`というトークンの並びを消費しつつ
  // アセンブリを出力
  while (!at_eof()) {
    int r_current = r_register();
    printf("  %%%d = load i32, i32* %%%d, align 4\n", r_current, r_main);
    int r_mid = r_register();
    if (consume('+')) {
      printf("  %%%d = add nsw i32 %%%d, %d\n", r_mid, r_current,
             expect_number());
    } else if (consume('-')) {
      printf("  %%%d = sub nsw i32 %%%d, %d\n", r_mid, r_current,
             expect_number());
    } else {
      error("予期しないトークンです: '%s'", token->str);
    }

    printf("  store i32 %%%d, i32* %%%d, align 4\n", r_mid, r_main);
  }

  int r_final_result = r_register();
  printf("  %%%d = load i32, i32* %%%d, align 4\n", r_final_result, r_main);
  int r_print_result = r_register();
  printf(
      "  %%%d = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef "
      "%%%d)\n",
      r_print_result, r_final_result);

  printf("  ret i32 0\n");

  printf("}\n");

  printf("\n");

  printf("declare i32 @printf(ptr noundef, ...) #1");

  return 0;
}