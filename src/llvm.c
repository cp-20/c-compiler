#include "llvm.h"

#include <stdlib.h>

int r_register() {
  static int i = 1;
  return i++;
}

int* r_register_ptr() {
  int* i = malloc(sizeof(int));
  *i = r_register();
  return i;
}
