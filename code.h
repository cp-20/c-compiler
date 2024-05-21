#pragma once

typedef struct Code {
  char* code;
  int size;
  int capacity;
} Code;

Code* init_code(void);

void push_code(Code* code, char* fmt, ...);

void merge_code(Code* code, Code* other);

void free_code(Code* code);
