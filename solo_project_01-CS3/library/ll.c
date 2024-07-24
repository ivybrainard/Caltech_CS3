#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ll.h"


typedef struct node {
    entry_t entry;
    struct node *next;
} node_t;

typedef struct ll_map {
    size_t length;
    node_t *head;
} ll_map_t;

/**
 * Initialize a new node with the given key and value.
 * 
 * The returned node should be heap-allocated with malloc and it takes ownership
 * of the arguments so the caller should not modify or free them afterward.
 */
static node_t *node_init(char *key, char* value);

/**
 * Frees a linked list node and all nodes it points to.
 * 
 * Also frees the keys and values since they are owned by the node.
 * 
 * Passing NULL to this function is valid (and will do nothing).
 */
static void node_free(node_t *curr);

static node_t *node_init(char *key, char* value) {
    node_t *curr = malloc(sizeof(node_t));
    curr->entry.key = key;
    curr->entry.value = value;
    curr->next = NULL;
    return curr;
}

static void node_free(node_t *curr) {
    while(curr != NULL) {
        node_t *new = curr->next;
        free(curr->entry.key);
        free(curr->entry.value);
        free(curr);
        curr = new;
    }
}

ll_map_t *ll_init(void) {
    ll_map_t *curr = malloc(sizeof(ll_map_t));
    curr->length = 0;
    curr->head = NULL;
    return curr;
}

void ll_free(ll_map_t *dict) {
    node_free(dict->head);
    free(dict);
}




/**
 * Adds a new key-value pair to the dict. If the key already exists, the value
 * is replaced and the old value is returned. Otherwise, NULL is returned.
 * 
 * Both the key and the value should be heap-allocated strings (created via 
 * malloc, calloc, strdup, or similar) and this function will take ownership of
 * them. The caller should not free or modify the key or value after passing
 * them to this function.
 * 
 * This function stores the string `value` as-is, rather than creating a new
 * allocation and copying it.
 * 
 * If an old value is returned, it should be exactly the string last placed at
 * this key by a previous `ll_put` call.
 * 
 * If the returned string is not NULL, it is heap-allocated and owned by the 
 * caller, so it is the caller's responsibility to free it when it is no longer 
 * needed.
 * */ 

char *ll_put(ll_map_t *dict, char *key, char *value) {

    if (dict->head == NULL) {
        dict->head = node_init(key, value);
        dict->length++;
        return NULL;
    }

    node_t *curr = dict->head;



    while (curr->next != NULL) {
        if (strcmp(curr->entry.key, key) == 0){
            char *oldEle = curr->entry.value;
            curr->entry.value = value;
            free(key);
            return oldEle;
        }
     
        curr = curr->next;
    }
    
    if (strcmp(curr->entry.key, key) == 0){
        char *oldELe = curr->entry.value;
        curr->entry.value = value;
        free(key);
        return oldELe;
    }

    curr->next = node_init(key, value);
    dict->length++;
    return NULL;
}

char *ll_get(ll_map_t *dict, char *key) {
    node_t *curr = dict->head;
    for(size_t i = 0; i < dict->length; i++) {
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
