#include "code.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "error.h"

#define CODE_SIZE_UNIT 1024

int sum_memory;

Code* init_code() {
  Code* code = calloc(1, sizeof(Code));
  if (code == NULL) {
    error("Failed to allocate memory\n");
  }
  // print_debug("calloc start");
  code->code = calloc(CODE_SIZE_UNIT, sizeof(char));
  // print_debug("calloc end");
  code->size = 0;
  code->capacity = CODE_SIZE_UNIT;
  sum_memory += CODE_SIZE_UNIT;
  // print_debug("sum_memory: %d (+%d)", sum_memory, CODE_SIZE_UNIT);
  return code;
}

void push_code(Code* code, char* fmt, ...) {
  if (code->size >= code->capacity - CODE_SIZE_UNIT) {
    code->capacity += CODE_SIZE_UNIT;
    // print_debug("push_code realloc start (capacity: %d)", code->capacity);
    char* new_code = realloc(code->code, code->capacity);
    // print_debug("push_code realloc end");
    if (new_code == NULL) {
      error("Failed to allocate memory\n");
    }
    code->code = new_code;
    sum_memory += CODE_SIZE_UNIT;
    // print_debug("sum_memory: %d (+%d)", sum_memory, CODE_SIZE_UNIT);
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
  if (code->size + other->size + 1 >= code->capacity - CODE_SIZE_UNIT) {
    int before = code->capacity;
    code->capacity = code->size + other->size + 1 + CODE_SIZE_UNIT;
    // print_debug("merge_code realloc start (capacity: %d -> %d)", before,
    //             code->capacity);
    char* new_code = realloc(code->code, code->capacity);
    // print_debug("merge_code realloc end");
    if (new_code == NULL) {
      error("Failed to allocate memory\n");
    }
    code->code = new_code;
    sum_memory += code->capacity - before;
    // print_debug("sum_memory: %d (+%d)", sum_memory, code->capacity - before);
  }

  strcat(code->code, other->code);
  code->size += other->size;

  free_code(other);
}

void free_code(Code* code) {
  sum_memory -= code->capacity;
  // print_debug("sum_memory: %d (-%d)", sum_memory, code->capacity);
  free(code->code);
  free(code);
}
