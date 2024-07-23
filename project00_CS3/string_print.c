#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void string_print(char *s) {
  size_t len = strlen(s);
  for (size_t i = 0; i < len; i++) {
    printf("%02zu: %c\n", i, s[i]);
  }
}

int main() {
  char *a = malloc(sizeof(char) * 4);
  char *b = malloc(sizeof(char) * 7);
  char *c = malloc(sizeof(char) * 21);

  strcpy(a, "CS3");
  strcpy(b, "Rules!");

  *c = '\0';

  strcat(c, a);
  strcat(c, b);

  string_print(c);

  free(a);
  free(b);
  free(c);

  return 0;
}
