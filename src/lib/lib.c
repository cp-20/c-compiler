#include "lib.h"

#include <stdio.h>
#include <stdlib.h>

void print(int n) { printf("%d\n", n); }

int scan() {
  int x;
  scanf("%d", &x);
  return x;
}

void alloc4(int** p, int a, int b, int c, int d) {
  *p = malloc(4 * sizeof(int));
  (*p)[0] = a;
  (*p)[1] = b;
  (*p)[2] = c;
  (*p)[3] = d;
}
