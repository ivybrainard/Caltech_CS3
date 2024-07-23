#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **string_blanks(char *s) {
  // Implement this!
  // Allocate space for a list of strings.
  // (Hint: you need one string for each character that gets replaced by '_')
  char **a = malloc(strlen(s) * 8);

  for (size_t i = 0; i < strlen(s); i++) {
    a[i] = malloc((strlen(s) + 1) * sizeof(char));
    strcpy(a[i], s);
    a[i][i] = '_';
    a[i][strlen(s)] = '\0';
  }
  return a;

  // Use strcpy to copy the contents of s into a new string, setting one of
  // the characters in the copy to '_'.

  // Add the edited copy to the list of strings.

  // Return the final list of strings with blanks.
}
