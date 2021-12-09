#include <stdio.h>
#include "pair.h"
#include <stdlib.h>

// constructor for pair
pair_t *pair_create(int first_, int second_) {
    pair_t *pair = (pair_t *) malloc(sizeof(pair_t));
    pair->first = first_;
    pair->second = second_;

    return pair;
}

// destructor for pair
void pair_free(pair_t *pair) {
    if (pair == NULL) {
        fprintf(stderr, "[ERROR] Can't free pair, pointer is NULL\n");
        return;
    }
    free(pair);
    pair = NULL;
}

// update values of pair
void pair_update(pair_t *pair, int first_, int second_) {
    pair->first = first_;
    pair->second = second_;
}
