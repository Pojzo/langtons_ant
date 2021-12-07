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

static SDL_Window *init_window() {
    SDL_Window *window = SDL_CreateWindow(
            WINDOW_NAME,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
            );

    return window;
}

// constructor for SDL_Renderer
static SDL_Renderer *init_renderer(SDL_Window *window) {
    if (window == NULL) {
        fprintf(stderr, "[ERROR] Could not initialize renderer, invalid pointer to SDL_Window\n");
    }
    // create an instance of renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//SDL_RENDERER_ACCELERATED 
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    return renderer;
}

// refresh background to white
static void refresh_screen(SDL_Renderer *renderer) {
    assert(renderer != NULL);
    // set color for renderer as white
    SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);

    // clear window, create a white background
    SDL_RenderClear(renderer);
}

static void free_all(SDL_Window *window, SDL_Renderer *renderer, ant_t *ant) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    ant_free(ant);
    SDL_Quit();
}

// returns true if user has pressed quit button
static int handle_quit() {
    SDL_Event evt;
    while(SDL_PollEvent(&evt) != 0) {
        switch (evt.type) {
            case SDL_QUIT:    
                return 1;
        }
    }
    return 0;
}

// update screen and handle quitting
static void update(SDL_Renderer *renderer, ant_t *ant) {
    grid_draw(renderer, ant->grid);
    SDL_RenderPresent(renderer);
    SDL_Delay(500);
    if (handle_quit()) {
        printf("Terminating program\n");
        return;
    }
}

// run simulation of langtons ant
static void simulate(SDL_Renderer *renderer, ant_t *ant, int iterations) {
    for (int i = 0; i < iterations; i++) {
        printf("Iterations: %d\n", i);
        refresh_screen(renderer);
        update(renderer, ant);
    }
}

// initialize window and renderer and run simulation for given number of iterations
void run_simulation(ant_t *ant, int iterations) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    window = init_window();
    if (window == NULL) {
        fprintf(stderr, "[ERROR] Could not initialize window\n");
        free_all(window, renderer, ant);
        return;
    }
    renderer = init_renderer(window);
    if (renderer == NULL) {
        fprintf(stderr, "[ERROR] Could not initialize renderer\n");
        free_all(window, renderer, ant);
        return;
    }
    simulate(renderer, ant, iterations);
    free_all(window, renderer, ant);
}


