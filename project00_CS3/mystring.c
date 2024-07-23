#include "mystring.h"
#include <stdlib.h>
#include <string.h>

strarray_t *strsplit(const char *str) {
  int wordCount = 0;
  int idx = 0;

  while (str[idx] != '\0') {
    while (str[idx] == ' ') {
      idx++;
    }
    if (str[idx] == '\0') {
      break;
    }
    wordCount++;
    while (str[idx] != ' ' && str[idx] != '\0') {
      idx++;
    }
  }

  strarray_t *array = malloc(sizeof(strarray_t));
  array->length = wordCount;
  array->data = malloc(wordCount * sizeof(char *));

  int num = 0;
  int num2 = 0;

  while (str[num] != '\0') {
    if (str[num] != ' ') {
      int start = num;
      while (str[num] != ' ' && str[num] != '\0') {
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
