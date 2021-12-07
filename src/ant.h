#ifndef ANT_H
#define ANT_H
#include "common.h"

typedef struct Ant {
    unsigned pos_x;
    unsigned pos_y;
    grid_t *grid;
} ant_t;

ant_t *ant_create(grid_t *grid);
void ant_free(ant_t *ant);
#endif
