#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ll.h"
#include <assert.h>


typedef struct node {
    entry_t entry;
    struct node *next;
} node_t;

typedef struct ll_map {
    size_t length;
    node_t *head;
} ll_map_t;


static node_t *node_init(char *key, char* value);


static void node_free(node_t *curr);

static node_t *node_init(char *key, char* value) {
    node_t *curr = malloc(sizeof(node_t));
    assert(curr != NULL);
    curr->entry.key = key;
    curr->entry.value = value;
    curr->next = NULL;
    return curr;
}

static void node_free(node_t *curr) {
    while (curr != NULL) {
        node_t *new = curr->next;
        free(curr->entry.key);
        free(curr->entry.value);
        free(curr);
        curr = new;
    }
}

ll_map_t *ll_init(void) {
    ll_map_t *curr = malloc(sizeof(ll_map_t));
    assert(curr != NULL);
    curr->length = 0;
    curr->head = NULL;
    return curr;
}

void ll_free(ll_map_t *dict) {
    node_free(dict->head);
    free(dict);
}


char *ll_put(ll_map_t *dict, char *key, char *value) {
    if (dict->head == NULL) {
        dict->head = node_init(key, value);
        dict->length++;
        return NULL;
    }
    node_t *curr = dict->head;
    while (curr->next != NULL) {
        if (strcmp(curr->entry.key, key) == 0){
            char *old_element = curr->entry.value;
            curr->entry.value = value;
            free(key);
            return old_element;
        }
        curr = curr->next;
    }
    if (strcmp(curr->entry.key, key) == 0){
        char *old_element = curr->entry.value;
        curr->entry.value = value;
        free(key);
        return old_element;
    }
    curr->next = node_init(key, value);
    dict->length++;
    return NULL;
}

char *ll_get(ll_map_t *dict, char *key) {
    node_t *curr = dict->head;
    for (size_t i = 0; i < dict->length; i++) {
        if (strcmp(curr->entry.key, key) == 0) {
            return curr->entry.value;
        }  
        curr = curr->next;
    }
    return NULL;
}

strarray_t *ll_get_keys(ll_map_t *dict) {
    strarray_t *keyList = strarray_init(dict->length);
    node_t *curr = dict->head;
    for (size_t i = 0; i < dict->length; i++) {
        keyList->data[i] = strdup(curr->entry.key);
        curr = curr->next;
    }
    return keyList;
}
