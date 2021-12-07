#include <stdio.h>
#include "ant.h"

// constructor for ant
ant_t *ant_create(grid_t *grid) {
    if (grid == NULL) {
        fprintf(stderr, "[ERROR] Failed to create ant, grid is NULL\n");
        return NULL;
    }
    ant_t *ant = (ant_t *) malloc(sizeof(ant_t));
    if (ant == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate space for ant\n");
        return NULL;
    }

    // put ant in the middle of the screen
    ant->pos_x = grid->w / 2;
    ant->pos_y = grid->h / 2;

    ant->grid = grid;

    return ant;
}

// free ant from memory
void ant_free(ant_t *ant) {
    grid_free(ant->grid);
    ant->grid = NULL;
    free(ant);
    ant = NULL;
}
