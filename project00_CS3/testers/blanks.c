#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "../string_blanks.h"

const int MAX_INPUT_SIZE = 10000;

int main(int argc, char *argv[]) {
    assert(argc == 2 && "usage: test_string_blanks file");
    char *buf = malloc(MAX_INPUT_SIZE);
    FILE *in = fopen(argv[1], "r");
    assert(in && "file not found");
    buf[fread(buf, sizeof(char), MAX_INPUT_SIZE, in) - 1] = '\0'; 
    fclose(in);

    char **result = string_blanks(buf);
    for (size_t i = 0; i < strlen(buf); i++) {
        printf("%zu: \"%s\"\n", i, result[i]);
        free(result[i]);
    }
    free(buf);
    free(result);
}
