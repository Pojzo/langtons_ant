#ifndef GRID_H
#define GRID_H
#include "SDL2/SDL.h"
#include <stdbool.h>

typedef struct Grid {
    int *state;  // matrix for storing tile states
    unsigned w; // width of grid
    unsigned h; // height of grid
    unsigned change_x; // x coordinate of the tile to be inverted
    unsigned change_y; // y coordinate of the tile to be inverted
    bool redraw;
} grid_t;


grid_t *grid_create(unsigned w_, unsigned h_); // constructor for grid
void grid_free(grid_t *grid); // free grid from memory
void grid_print(const grid_t *grid);
// static void grid_assert(const grid_t *grid);
void grid_draw(SDL_Renderer *renderer, grid_t *grid, unsigned ant_x, unsigned ant_y);
unsigned grid_get(const grid_t *grid, unsigned x, unsigned y);
void grid_flip_color(grid_t *grid, unsigned x, unsigned y);

#endif
