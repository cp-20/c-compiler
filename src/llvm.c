#include "llvm.h"

#include <stdlib.h>

typedef int* rctx;

rctx r_init() {
  int* i = malloc(sizeof(int));
  *i = 0;
  return i;
}

void r_free(rctx rctx) { free(rctx); }

int r_register(rctx rctx) { return (*rctx)++; }

int* r_register_ptr(rctx rctx) {
  int* i = malloc(sizeof(int));
  *i = r_register(rctx);
  return i;
}
