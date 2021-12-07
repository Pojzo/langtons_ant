#include <stdio.h>
#include "common.h"
#include "grid.h"

SDL_Window *init_window();
SDL_Renderer *init_renderer(SDL_Window *window);
void refresh_screen(SDL_Renderer *renderer);
int handle_quit();
void free_all(SDL_Window *window, SDL_Renderer *renderer, grid_t *grid);

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    grid_t *grid = grid_create(10, 10);
    grid_print(grid);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    window = init_window();
    if (window == NULL) {
        fprintf(stderr, "[ERROR] Could not initialize window\n");
        free_all(window, renderer, grid);
        return 1;
    }
    renderer = init_renderer(window);
    if (renderer == NULL) {
        fprintf(stderr, "[ERROR] Could not initialize renderer\n");
        free_all(window, renderer, grid);
        return 1;
    }
    for (int i = 0; i < 10; i++) {
        refresh_screen(renderer);
        grid_draw(renderer, grid);
        SDL_RenderPresent(renderer);
        SDL_Delay(500);
        if (handle_quit()) {
            printf("Terminating program\n");
            free_all(window, renderer, grid);
            return EXIT_SUCCESS;
        }
    }
    free_all(window, renderer, grid);
    return 0;
}

SDL_Window *init_window() {
    SDL_Window *window = SDL_CreateWindow(
            WINDOW_NAME,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
            );

    return window;
}

// constructor for SDL_Renderer
SDL_Renderer *init_renderer(SDL_Window *window) {
    if (window == NULL) {
        fprintf(stderr, "[ERROR] Could not initialize renderer, invalid pointer to SDL_Window\n");
    }
    // create an instance of renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);//SDL_RENDERER_ACCELERATED 
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    return renderer;
}

// refresh background to white
void refresh_screen(SDL_Renderer *renderer) {
    assert(renderer != NULL);
    // set color for renderer as white
    SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);

    // clear window, create a white background
    SDL_RenderClear(renderer);
}

// returns true if user has pressed quit button
int handle_quit() {
    SDL_Event evt;
    while(SDL_PollEvent(&evt) != 0) {
        switch (evt.type) {
            case SDL_QUIT:    
                return 1;
        }
    }
    return 0;
}

void free_all(SDL_Window *window, SDL_Renderer *renderer, grid_t *grid) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    grid_free(grid);
    SDL_Quit();
}

