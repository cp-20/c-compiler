#define NULL 0

char* concat_string(char** strings) {
  int length = 0;
  for (int i = 0; strings[i] != NULL; i++) {
    length += strlen(strings[i]);
  }

  char* buffer = malloc(length + 1);
  buffer[0] = '\0';

  for (int i = 0; strings[i] != NULL; i++) {
    strcat(buffer, strings[i]);
  }

  return buffer;
}