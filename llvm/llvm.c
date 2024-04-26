#include <stdio.h>
#include <string.h>

char* print_header(char* name) {
  printf("source_filename = \"%s\"\n", name);
  printf(
      "target datalayout = "
      "\"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-"
      "S128\"\n");
  printf("target triple = \"x86_64-unknown-linux-gnu\"\n");
  printf("\n");
}

int register_register() {
  static int i = 0;
  return i++;
}

int register_string() {
  static int i = 0;
  return i++;
}
