#pragma once

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

Token *new_token(TokenKind kind, Token *cur, char *str);

bool is_space(char c);

bool is_digit(char c);

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p);

bool consume(Token **token, char *op);

void expect(Token **token, char *op);

int expect_number(Token **token);