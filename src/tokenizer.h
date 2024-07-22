#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

// トークンの種類
typedef enum {
  TK_RESERVED,  // 記号
  TK_IDENT,     // 識別子
  TK_NUM,       // 整数トークン
  TK_RETURN,    // return
  TK_IF,        // if
  TK_ELSE,      // else
  TK_WHILE,     // while
  TK_FOR,       // for
  TK_SIZEOF,    // sizeof
  TK_INT,       // int
  TK_VOID,      // void
  TK_STRUCT,    // struct
  TK_ENUM,      // enum
  TK_TYPEDEF,   // typedef
  TK_EOF,       // 入力の終わりを表すトークン
} TokenKind;

// トークンの型
typedef struct Token Token;
struct Token {
  TokenKind kind;  // トークンの型
  Token *next;     // 次の入力トークン
  int val;         // kindがTK_NUMの場合、その数値
  char *str;       // トークン文字列
  int len;         // トークンの長さ
};

Token *new_token(TokenKind kind, Token *cur, char *str);

bool is_space(char c);

bool is_digit(char c);

bool is_alnum(char c);

bool is_special(char c);

bool is_special2(char *p);

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p);

bool consume_reserved(Token **token, TokenKind kind);

bool consume(Token **token, char *op);

int consume_number(Token **token);

Token *consume_ident(Token **token);

void expect(Token **token, char *op);

int expect_number(Token **token);
