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

typedef struct LVar LVar;

// ローカル変数の型
struct LVar {
  LVar *next;  // 次の変数かNULL
  char *name;  // 変数の名前
  int len;     // 名前の長さ
  int offset;  // RBPからのオフセット
};

// ローカル変数
extern LVar *locals;

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok);

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

Token *consume_ident(Token **token);

void expect(Token **token, char *op);

int expect_number(Token **token);