#include <stdio.h>
#include <stdbool.h>

#include "ant.h"
#include "pair.h"

static int QUIT = 0;
static int PAUSED = 0;
static const int transformations_x[] = {0, 1, 0, -1};
static const int transformations_y[] = {-1, 0, 1, 0};

unsigned int number_of_steps;

static bool KEYS[322];

// constructor for ant
ant_t *ant_create(grid_t *grid) {
    // check if grid is properly initialized
    if (grid == NULL) {
        fprintf(stderr, "[ERROR] Failed to create ant, grid is NULL\n");
        return NULL;
    }
    ant_t *ant = (ant_t *) malloc(sizeof(ant_t));
    // check if malloc failed
    if (ant == NULL) {
        fprintf(stderr, "[ERROR] Failed to allocate space for ant\n");
        return NULL;
    }

    // put ant in the middle of the screen
    ant->pos_x = grid->w / 2;
    ant->pos_y = grid->h / 2;

    ant->transformation = 0;
    pair_t *direction = pair_create(transformations_x[ant->transformation], transformations_y[ant->transformation]);
    if (direction == NULL) {
        return NULL;
    }

    ant->grid = grid;
    ant->direction = direction;

    return ant;
}

// free ant from memory
void ant_free(ant_t *ant) {
    grid_free(ant->grid); // free grid
    ant->grid = NULL;

    pair_free(ant->direction); // free direction
    ant->direction = NULL;

    free(ant); // free ant
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

// rotate ant by 90 degrees to the left
static void ant_turn_right(ant_t *ant) {
    if (ant == NULL || ant->direction == NULL) {
        fprintf(stderr, "[ERROR] Ant not initalized\n");
        return;
    }
    // if we're on the final transformation, return to the first one
    if (ant->transformation == 3) {
        ant->transformation = 0;
    }
    else {
        ant->transformation++; 
    }

    int new_x = transformations_x[ant->transformation];
    int new_y = transformations_y[ant->transformation];

    pair_update(ant->direction, new_x, new_y);
}

static void ant_turn_left(ant_t *ant) {
    if (ant == NULL || ant->direction == NULL) {
        fprintf(stderr, "[ERROR] Ant not initalized\n");
        return;
    }
    // if we're on the first transforation, return to the last one
    if (ant->transformation == 0) {
        ant->transformation = 3;
    }
    else {
        ant->transformation--;
    }

    int new_x = transformations_x[ant->transformation];
    int new_y = transformations_y[ant->transformation];

    pair_update(ant->direction, new_x, new_y);
}

void ant_move_forward(ant_t *ant) {
    if (ant == NULL || ant->direction == NULL) {
        fprintf(stderr, "[ERROR] Ant not initalized\n");
        return;
    }
    int new_x = ant->pos_x + ant->direction->first;
    int new_y = ant->pos_y + ant->direction->second;
    // printf("%d %d\n", ant->direction->first, ant->direction->second);
    
    // printf("New_x: %d, New_y: %d\n", new_x, new_y);
    if (new_x < 0 || new_x > (int) ant->grid->h - 1) {
        QUIT = 1;
        return;
    }

    if (new_y < 0 || new_y > (int) ant->grid->h - 1) {
        QUIT = 1;
        return;
    }
    ant->pos_x = new_x;
    ant->pos_y = new_y;
}


// free all allocated memory
static void free_all(SDL_Window *window, SDL_Renderer *renderer, ant_t *ant) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    ant_free(ant);
    SDL_Quit();
}

static void handle_events() {
    SDL_Event evt;
    while (SDL_PollEvent(&evt) != 0)
    {
        switch (evt.type)
        {
        case SDL_QUIT:
            QUIT = 1;
            break;
        case SDL_KEYDOWN:
            PAUSED = !PAUSED;
            break;
        }
    }
}

// update screen and handle quitting
static void update(SDL_Renderer *renderer, ant_t *ant)
{
    grid_draw(renderer, ant->grid, ant->pos_x, ant->pos_y); // draw grid
    SDL_RenderPresent(renderer);    // render grid
    SDL_Delay(DELAY);               
    if (KEYS[SDLK_SPACE]) {
        printf("Space has been pressed\n");
        KEYS[SDLK_SPACE] = !KEYS[SDLK_SPACE];
    }
}

// run simulation of langtons ant
static void simulate(SDL_Renderer *renderer, ant_t *ant, int iterations)
{
    // SDL_RenderPresent(renderer);
    // grid_draw(renderer, ant->grid);

    ant->grid->redraw = false;
    int i;
    for (i = 0; i < iterations; i++)
    {
        if (QUIT)
            return;
        
        if (grid_get(ant->grid, ant->pos_x, ant->pos_y) == 0)
        {
            ant_turn_left(ant);
        }
        else
        {
            ant_turn_right(ant);
        }
        grid_flip_color(ant->grid, ant->pos_x, ant->pos_y);
        ant_move_forward(ant);

        refresh_screen(renderer);
        while (PAUSED) {
            handle_events();
            if (QUIT) {
                break;
            }
        }
        handle_events();
        update(renderer, ant);
    }
    printf("Ending the simulation after %d steps\n", i);
}

// reset all keys to false, which means not pressed
static void reset_keys()
{
    for (int i = 0; i < 322; i++)
    {
        KEYS[i] = false;
    }
}

// initialize window and renderer and run simulation for given number of iterations
void run_simulation(ant_t *ant, int iterations)
{
    reset_keys();
    number_of_steps = 0;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    window = init_window(); // initialize SDL_Window
    if (window == NULL)
    { // check if window initialization failed
        fprintf(stderr, "[ERROR] Could not initialize window\n");
        free_all(window, renderer, ant);
        return;
    }
    renderer = init_renderer(window); // initialized SDL_Renderer
    if (renderer == NULL)
    { // check if renderer initialization failed
        fprintf(stderr, "[ERROR] Could not initialize renderer\n");
        free_all(window, renderer, ant);
        return;
    }
    // run simulation for given number of iterations
    simulate(renderer, ant, iterations);

    // free everything from memory
    free_all(window, renderer, ant);
}
