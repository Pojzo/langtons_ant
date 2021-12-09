#ifndef ANT_H
#define ANT_H
#include "pair.h"
#include "common.h"

typedef struct Ant {
    unsigned pos_x;
    unsigned pos_y;
    unsigned transformation;
    pair_t *direction;
    grid_t *grid;
} ant_t;


ant_t *ant_create(grid_t *grid);
void ant_free(ant_t *ant);
void run_simulation(ant_t *ant, int iterations);
#endif
