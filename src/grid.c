#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "grid.h"
#include "config.h"

// constructor for grid
grid_t *grid_create(unsigned w_, unsigned h_) {
    // if creating grid greater than maximum allowed width
    if (w_ > MAX_GRID_WIDTH) {
        fprintf(stderr, "[ERROR] Width exceedes maximum allowed width for grid\n");
        return NULL;
    }

    // if creating grid greater than maximum allowed height
    if (h_ > MAX_GRID_HEIGHT) {
        fprintf(stderr, "[ERROR] Height exceedes maximum allowed height for grid\n");
        return NULL;
    }

    grid_t *grid = (grid_t *) malloc(sizeof(grid_t)); // allocate space for pointer to Grid struct
    if (grid == NULL) { // check if malloc for grid failed
        fprintf(stderr, "[ERROR] Failed to allocate memory for grid\n");
        return NULL;
    }
    grid->w = w_;
    grid->h = h_;
    grid->state = (int *) malloc(sizeof(int) * grid->w * grid->h);
    memset(grid->state, 0, (grid->w * grid->h) * sizeof(int));

    if (grid->state == NULL) {  // check if malloc for grid state matrix failed
        fprintf(stderr, "[ERROR] Failed to allocate memory for grid states\n");
        return NULL;
    }

    return grid;
}

// free grid structure from memory
void grid_free(grid_t *grid) {
    grid_assert(grid);

    free(grid->state); 
    grid->state = NULL;

    free(grid);
    grid = NULL;
}


// assert if grid and grid states is not null
void grid_assert(const grid_t *grid) {
    assert(grid != NULL);
    assert(grid->state != NULL);
}

// return value at x and y coordinate 
unsigned grid_get(const grid_t *grid, unsigned x, unsigned y) {
    grid_assert(grid);
    return grid->state[x * grid->h + y];
}

// print grid to stdout
void grid_print(const grid_t *grid) {
    grid_assert(grid);
    for (unsigned i = 0; i < grid->w; i++) {
        for (unsigned j = 0; j < grid->h; j++) {
            printf("%d%c", grid_get(grid, i, j), j == grid->h - 1 ? '\n' : ' ');
        }
    }
}

