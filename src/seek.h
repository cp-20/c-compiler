#pragma once

#include <stdbool.h>

bool exists_file(char* path);
char* read_file(char* path);
char* write_file(char* path, char* content);
