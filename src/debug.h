#pragma once

#include <stdbool.h>

extern bool f_debug;
extern bool f_suppress;

void print_debug(char* fmt, ...);
