#ifndef PAIR_H
#define PAIR_H

typedef struct Pair {
    int first; // first item of pair
    int second; // second item of pair
} pair_t;

pair_t *pair_create(int first_, int second_);
void pair_free(pair_t *pair);

#endif
