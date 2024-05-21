#include "tokenizer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"

// ローカル変数
LVar *locals;

// 変数を名前で検索する。見つからなかった場合はNULLを返す。
LVar *find_lvar(Token *tok) {
  for (LVar *var = locals; var; var = var->next)
    if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
      return var;
  return NULL;
}

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

bool is_alnum(char c) {
  if ('a' <= c && c <= 'z') return true;
  if ('A' <= c && c <= 'Z') return true;
  if (c == '_') return true;
  return false;
}

bool is_special(char c) {
  if (c == '+') return true;
  if (c == '-') return true;
  if (c == '*') return true;
  if (c == '/') return true;
  if (c == '(') return true;
  if (c == ')') return true;
  if (c == '<') return true;
  if (c == '>') return true;
  if (c == '=') return true;
  if (c == ';') return true;
  if (c == '{') return true;
  if (c == '}') return true;
  return false;
}

bool is_special2(char *p) {
  if (strncmp(p, "==", 2) == 0) return true;
  if (strncmp(p, "!=", 2) == 0) return true;
  if (strncmp(p, "<=", 2) == 0) return true;
  if (strncmp(p, ">=", 2) == 0) return true;
  return false;
}

// 入力文字列pをトークナイズしてそれを返す
Token *tokenize(char *p) {
  // 初期化
  locals = NULL;

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
    if (is_special2(p)) {
      cur = new_token(TK_RESERVED, cur, p);
      cur->len = 2;
      p += 2;
      continue;
    }

    // 1文字の記号
    if (is_special(*p)) {
      cur = new_token(TK_RESERVED, cur, p++);
      cur->len = 1;
      continue;
    }

    // return文
    if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
      cur = new_token(TK_RETURN, cur, p);
      p += 6;
      continue;
    }

    // if文
    if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
      cur = new_token(TK_IF, cur, p);
      p += 2;
      continue;
    }

    // else文
    if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
      cur = new_token(TK_ELSE, cur, p);
      p += 4;
      continue;
    }

    // while文
    if (strncmp(p, "while", 5) == 0 && !is_alnum(p[5])) {
      cur = new_token(TK_WHILE, cur, p);
      p += 5;
      continue;
    }

    // for文
    if (strncmp(p, "for", 3) == 0 && !is_alnum(p[3])) {
      cur = new_token(TK_FOR, cur, p);
      p += 3;
      continue;
    }

    // 識別子
    if (is_alnum(*p)) {
      char *start = p;
      do {
        p++;
      } while (is_alnum(*p) || is_digit(*p));

      cur = new_token(TK_IDENT, cur, start);
      cur->len = p - start;
      continue;
    }

    // 数字
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

bool consume_reserved(Token **token, TokenKind kind) {
  if ((*token)->kind != kind) return false;
  (*token) = (*token)->next;
  return true;
}

bool consume(Token **token, char *op) {
  if ((*token)->kind != TK_RESERVED || strlen(op) != (*token)->len ||
      memcmp((*token)->str, op, (*token)->len))
    return false;
  (*token) = (*token)->next;
  return true;
}

Token *consume_ident(Token **token) {
  if ((*token)->kind != TK_IDENT) return NULL;
  Token *cur = *token;
  (*token) = (*token)->next;
  return cur;
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