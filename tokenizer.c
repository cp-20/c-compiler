#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

// トークンの種類
typedef enum {
  TK_RESERVED,  // 記号
  TK_NUM,       // 整数トークン
  TK_EOF,       // 入力の終わりを表すトークン
} TokenKind;

// トークンの型
typedef struct Token {
  TokenKind kind;      // トークンの型
  struct Token *next;  // 次の入力トークン
  int val;             // kindがTK_NUMの場合、その数値
  char *str;           // トークン文字列
  int len;             // トークンの長さ
} Token;

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

// 入力文字列pをトークナイズしてそれを返す
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

    // 2文字の記号
    if (strncmp(p, "==", 2) == 0 || strncmp(p, "!=", 2) == 0 ||
        strncmp(p, "<=", 2) == 0 || strncmp(p, ">=", 2) == 0) {
      cur = new_token(TK_RESERVED, cur, p);
      cur->len = 2;
      p += 2;
      continue;
    }

    // 1文字の記号
    if (strchr("+-*/()<>", *p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      cur->len = 1;
      continue;
    }

    if (is_digit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(p, "トークナイズできません");
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

bool consume(Token **token, char *op) {
  if ((*token)->kind != TK_RESERVED || strlen(op) != (*token)->len ||
      memcmp((*token)->str, op, (*token)->len))
    return false;
  (*token) = (*token)->next;
  return true;
}

void expect(Token **token, char *op) {
  if ((*token)->kind != TK_RESERVED || strlen(op) != (*token)->len ||
      memcmp((*token)->str, op, (*token)->len))
    error_at((*token)->str, "'%s'ではありません", op);
  *token = (*token)->next;
}

int expect_number(Token **token) {
  if ((*token)->kind != TK_NUM) error_at((*token)->str, "数ではありません");
  int val = (*token)->val;
  *token = (*token)->next;
  return val;
}