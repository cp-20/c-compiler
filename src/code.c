#include "code.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "error.h"

#define CODE_SIZE_UNIT 2048

Code* init_code() {
  Code* code = malloc(sizeof(Code));
  code->code = calloc(CODE_SIZE_UNIT, sizeof(char));
  code->size = 0;
  code->capacity = CODE_SIZE_UNIT;
  return code;
}

void push_code(Code* code, char* fmt, ...) {
  if (code->size >= code->capacity - CODE_SIZE_UNIT) {
    code->capacity += CODE_SIZE_UNIT;
    char* new_code = realloc(code->code, code->capacity);
    if (new_code == NULL) {
      error("Failed to allocate memory\n");
    }
    code->code = new_code;
  }

  va_list ap;
  va_start(ap, fmt);

  int chars_written =
      vsnprintf(code->code + code->size, code->capacity - code->size, fmt, ap);
  if (chars_written < 0) {
    error("Failed to write to code buffer\n");
  } else if (code->size + chars_written >= code->capacity) {
    error("Code buffer overflow\n");
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
