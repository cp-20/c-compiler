#include "code.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Code* init_code() {
  Code* code = malloc(sizeof(Code));
  code->code = calloc(65536, sizeof(char));
  code->size = 0;
  code->capacity = 65536;
  return code;
}

void push_code(Code* code, char* fmt, ...) {
  if (code->size + 1 >= code->capacity) {
    code->capacity = (code->capacity == 0) ? 1 : code->capacity * 2;
    char* new_code = realloc(code->code, code->capacity);
    if (new_code == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
    }
    code->code = new_code;
  }

  va_list ap;
  va_start(ap, fmt);

  int chars_written =
      vsnprintf(code->code + code->size, code->capacity - code->size, fmt, ap);
  if (chars_written < 0) {
    fprintf(stderr, "Error occurred while formatting string\n");
    exit(EXIT_FAILURE);
  }

  code->size += chars_written;
  va_end(ap);
}

void merge_code(Code* code, Code* other) {
  if (code->size + other->size + 1 >= code->capacity) {
    code->capacity = code->capacity * 2 + other->size + 1;
    code->code = realloc(code->code, code->capacity);
  }

  sprintf(code->code + code->size, "%s", other->code);
  code->size += other->size;

  free_code(other);
}

void free_code(Code* code) {
  free(code->code);
  free(code);
}
