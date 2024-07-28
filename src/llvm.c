#include "llvm.h"

#include <stdlib.h>

int* r_init() {
  int* i = malloc(sizeof(int));
  *i = 0;
  return i;
}

void r_free(int* rctx) { free(rctx); }

int r_register(int* rctx) { return (*rctx)++; }

int* r_register_ptr(int* rctx) {
  int* i = malloc(sizeof(int));
  *i = r_register(rctx);
  return i;
}
