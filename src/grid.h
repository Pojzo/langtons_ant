#ifndef GRID_H
#define GRID_H

typedef struct Grid {
    int *state;  // matrix for storing tile states
    unsigned w; // width of grid
    unsigned h; // height of grid
} grid_t;

grid_t *grid_create(unsigned w_, unsigned h_); // constructor for grid
void grid_free(grid_t *grid); // free grid from memory
void grid_print(const grid_t *grid);
unsigned grid_get(const grid_t *grid, unsigned x, unsigned y);
void grid_assert(const grid_t *grid);

#endif
