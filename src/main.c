#include <stdio.h>
#include <stdbool.h>
#include "common.h"
#include "ant.h"
#include "config.h"

// number of iterations to run the simulation for
static const int iterations = 2000;

// returns true if a given string is a number, can be both positive and negative
inline static bool is_number(char* string) {
    for (size_t i = 0; i < strlen(string); i++) {
        if (string[i] < 48 || string[i] > 57) return false;
    }
    return true;
}

// set the values for grid width and grid height either for default values, or parse them from command line arguments
int parse_args(unsigned *grid_width, unsigned *grid_height, int argc, char *argv[]) {
    *grid_width = DEFAULT_GRID_WIDTH;
    *grid_height = DEFAULT_GRID_HEIGHT;

    if (argc == 1) {
        return 0;
    }
    if (argc != 3) {
        fprintf(stderr, "Invalid number of arguments supplied\n");
        return 1;
    }
    if (!is_number(argv[1]) || !is_number(argv[2])) {
        fprintf(stderr, "Arguments have to be integers\n");
        return 1;
    }
    int grid_width_unsafe = atoi(argv[1]);
    int grid_height_unsafe = atoi(argv[2]);
    if ((grid_width_unsafe < (int)DEFAULT_GRID_WIDTH || grid_width_unsafe > (int)MAX_GRID_WIDTH) ||
        (grid_height_unsafe < (int)DEFAULT_GRID_HEIGHT || grid_height_unsafe > (int)MAX_GRID_HEIGHT))
    {

        fprintf(stderr, "Invalid range for the grid size\n");
        return 1;
    }
    *grid_width = (unsigned)grid_width_unsafe;
    *grid_height = (unsigned)grid_height_unsafe;

    return 0;
}

static unsigned adjust_size(unsigned size, unsigned screen_size) {
    unsigned remainder = screen_size % size;
    return screen_size - remainder;
}


static void adjust_screen_size(unsigned grid_rows, unsigned grid_cols) {
    if (SCREEN_WIDTH % grid_cols != 0) {
        SCREEN_WIDTH = adjust_size(grid_cols, SCREEN_WIDTH);
    }
    if (SCREEN_HEIGHT % grid_rows != 0) {
        SCREEN_HEIGHT = adjust_size(grid_rows, SCREEN_HEIGHT);
    }
}

// entry point of the program
int main(int argc, char *argv[])
{
    unsigned int grid_width;
    unsigned int grid_height;

    if (parse_args(&grid_width, &grid_height, argc, argv)) {
        printf("Failed to parse args, defaulting to grid_width: %d, grid_height: %d\n", (int)grid_width, (int)grid_height);
    }
    printf("Screen size before adjusting; (%d: %d)\n", SCREEN_WIDTH, SCREEN_HEIGHT);
    adjust_screen_size(grid_width, grid_height);
    printf("Screen size after adjusting; (%d: %d)\n", SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Init(SDL_INIT_EVERYTHING);

    grid_t *grid = grid_create(grid_width, grid_height);

    ant_t *ant = ant_create(grid);
    run_simulation(ant, iterations);

    return 0;
}
