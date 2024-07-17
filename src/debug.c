#include "debug.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "color.h"

bool f_debug = false;

void print_debug(char* fmt, ...) {
  if (!f_debug) return;

  va_list ap;
  va_start(ap, fmt);
  printf(COL_MAGENTA "[DEBUG] " COL_RESET);
  vprintf(fmt, ap);
  printf("\n");
  va_end(ap);
}
