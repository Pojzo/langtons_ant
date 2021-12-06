#include <stdio.h>
#include "grid.h"

int main() {
    grid_t *grid = grid_create(5, 5);
    grid_print(grid);
    grid_free(grid);
    return 0;
}
