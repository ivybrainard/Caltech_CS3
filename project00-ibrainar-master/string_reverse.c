#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *string_reverse(char *s) {
  // Implement this!
  // Use strlen to allocate enough memory for the string, including the
  // null-terminator.
  char *a = malloc(strlen(s) + 1);

  // Set the null-terminator byte.
  a[strlen(s)] = '\0';

  // Write a for loop to reverse the string. You can index into the
  // string just like an array to get the characters of s as well as
  // set the characters in the reversed string.
  for (size_t i = 0; i < strlen(s); i++) {
    a[i] = s[strlen(s) - i - 1];
  }

  // Return the reversed string.
  return a;
}
