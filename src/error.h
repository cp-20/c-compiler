#pragma once

#include <stdarg.h>
#include <stdio.h>

extern char *user_input;
extern char *filename;

void init_error(char *p);

// エラーを報告する
void error(char *fmt, ...);

// エラー箇所を報告する
void error_at(char *loc, char *fmt, ...);
