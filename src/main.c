#include <stdio.h>
#include "common.h"
#include "ant.h"


int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    grid_t *grid = grid_create(10, 10);
    ant_t *ant = ant_create(grid);
    const int iterations = 50;
    run_simulation(ant, iterations);

    return 0;
}

