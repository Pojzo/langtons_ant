#include <stdio.h>
#include "common.h"
#include "ant.h"

const int iterations = 2000;

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    grid_t *grid = grid_create(40, 40);
    ant_t *ant = ant_create(grid);
    run_simulation(ant, iterations);

    return 0;
}

