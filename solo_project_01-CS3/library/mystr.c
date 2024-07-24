#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "strarray.h"
#include "mystr.h"


/**
 * Returns the index of the first occurrence of sep in str starting from start.
 * 
 * If sep is not found, returns -1.
 * 
 * Example:
 * ```
 * assert(strindexof("hello", 'l', 0) == 2);
 * assert(strindexof("hello", 'l', 3) == 3);
 * assert(strindexof("hello", 'l', 4) == -1);
 * ```
 */
ssize_t mystr_indexof(const char *str, const char sep, size_t start) {
    int num = 0;
    for(size_t i = 0; i < strlen(str); i++) {
        if(str[i] == sep) {
          if(i == start) {
            return i;
          } else {
            if(start < i) {
              return i;
            }
          }
        }
    }
    if(num == 0) {
      return -1;
    }
    return -1;
}


/**
 * Splits a string into an array of strings using sep as the delimiter. Leading,
 * trailing, and repeated seperators should eliminated (i.e., there should be no
 * empty strings in the output).
 * 
 * The original string is not modified. The returned array and the strings
 * inside it are heap-allocated and should be freed by the caller using
 * `strarray_free` when they are no longer needed.
 */
strarray_t *mystr_split(const char *str, const char sep) {

  int wordCount = 0;
  int idx = 0;

  while (str[idx] != '\0') {
    while (str[idx] == sep) {
      idx++;
    }
    if (str[idx] == '\0') {
      break;
    }
    wordCount++;
    while (str[idx] != sep && str[idx] != '\0') {
      idx++;
    }
  }
  strarray_t *array = malloc(sizeof(strarray_t));
  array->length = wordCount;
  array->data = malloc(wordCount * sizeof(char *));

  int num = 0;
  int num2 = 0;

  while (str[num] != '\0') {
    if (str[num] != sep) {
      int start = num;
      while (str[num] != sep && str[num] != '\0') {
        num++;
      }

      int l = num - start;
      array->data[num2] = malloc(l + 1);
      strncpy(array->data[num2], str + start, l);
      array->data[num2][l] = '\0';
      num2++;
    } else {
      num++;
    }
  }

  return array;
  



}
