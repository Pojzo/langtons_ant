#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "grid.h"
#include "config.h"


// print common error message
static void grid_null_error() {
    fprintf(stderr, "[ERROR] Grid not initialized\n");
}

// check if grid has been initialized
static int grid_initialized(const grid_t *grid) {
    if (grid == NULL) {
        grid_null_error();
        return 0;
    }
    if (grid->state == NULL) {
        grid_null_error();
        return 0;
    }
    return 1;
}

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
    grid->change_x = -1;
    grid->change_y = -1;
    grid->redraw = true; // if redraw is set to true, the whole maze is drawn, otherwise only the change_x and change_y
                         // squars
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
    if (grid == NULL) {
        return;
    }
    free(grid->state); 
    grid->state = NULL;

    free(grid);
    grid = NULL;
}


// assert if grid and grid states is not null
/*
static void grid_assert(const grid_t *grid) {
    assert(grid != NULL);
    assert(grid->state != NULL);
}
*/

// return value at x and y coordinate 
unsigned grid_get(const grid_t *grid, unsigned x, unsigned y) {
    if (!grid_initialized(grid)) {
        grid_null_error();
        return INT_MAX;
    }
    return grid->state[x * grid->h + y];
}

unsigned grid_get_index(const grid_t *grid, unsigned x, unsigned y) {
    if (!grid_initialized(grid)) {
        grid_null_error();
        return INT_MAX;
    }
    return x * grid->h + y;
}



// print grid to stdout
void grid_print(const grid_t *grid) {
    if (!grid_initialized(grid)) {
        grid_null_error();
    }
    for (unsigned i = 0; i < grid->w; i++) {
        for (unsigned j = 0; j < grid->h; j++) {
            printf("%d%c", grid_get(grid, i, j), j == grid->h - 1 ? '\n' : ' ');
        }
    }
}

// return SDL_Rect with dimensions of one tile in grid
static SDL_Rect rect_create(unsigned x_tiles, unsigned y_tiles) {
    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = SCREEN_WIDTH / x_tiles;
    r.h = SCREEN_HEIGHT / y_tiles;

    return r;
}

// flip color at index x, y
void grid_flip_color(grid_t *grid, unsigned x, unsigned y) {
    if (!grid_initialized(grid)) {
        grid_null_error();
        return;
    }
    // printf("%d %d\n", x, y);
    int index = grid_get_index(grid, x, y);
    if (index == INT_MAX) {
        fprintf(stderr, "[ERROR] Invalid indeces to swap color\n");
        return;
    }
    // swap between zero and one
    // printf("%d\n", index);
    grid->state[index] = 1 - grid->state[index];
}

// draw grid to SDL_Renderer
void grid_draw(SDL_Renderer *renderer, grid_t *grid) {
    srand(time(NULL));
    if (!grid_initialized(grid)) {
        grid_null_error();
    }

    SDL_Rect r = rect_create(grid->w, grid->h);
    for (unsigned i = 0; i < grid->w; i++) {
        for (unsigned j = 0; j < grid->h; j++) {
            if (grid_get(grid, i, j) == 0) {
                // SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
                SDL_SetRenderDrawColor(renderer, BORDER_R, BORDER_G, BORDER_B, BORDER_A);
                //SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, rand() % 255);
                SDL_RenderDrawRect(renderer, &r);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderFillRect(renderer, &r);
            }
            r.x += r.w;
        }
        r.x = 0;
        r.y += r.h;
    }
}
