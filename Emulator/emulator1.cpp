#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} sdl_t;

typedef struct {
    uint32_t window_height;
    uint32_t window_width;
    uint32_t fg_color;
    uint32_t bg_color;
    uint32_t scale_factor; //amount to scale the CHIP8  pixel
} config_t;

bool init_sdl(sdl_t* sdl, const config_t* config) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        SDL_Log("could not initialize SDL subsystems! %s\n", SDL_GetError());
        return false;
    }

    sdl->window = SDL_CreateWindow(
        "CHIP8 output",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config -> window_width * config->scale_factor,
        config -> window_height * config->scale_factor,
        0
    );

    if (!sdl->window) {
        SDL_Log("could not create SDL window %s \n", SDL_GetError());
        return false;
    }

    sdl->renderer = SDL_CreateRenderer(
        sdl->window, 
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!sdl->renderer) {
        SDL_Log("could not create SDL window %s \n", SDL_GetError());
        return false;
    }

    SDL_Delay(3000);
    return true;
}

bool set_config_from_args(config_t* config, const int argc, char **argv) {
    config->window_height = 480;
    config->window_width = 640;
    config->fg_color = 0xFFFFFFFF; //white
    config->bg_color = 0xFFFF00FF; //yellow
    config->scale_factor = 5; //default resolution 1240 x 60

    for (int i = 1; i < argc; i++) {
        (void)argv[i]; // prevent compiler error from unused variables
    }

    return true;
}


void final_cleanup(sdl_t* sdl) {
    SDL_DestroyRenderer(sdl->renderer);
    SDL_DestroyWindow(sdl->window);
    SDL_Quit();
}

void clear_screen(const config_t config, sdl_t* sdl){
    const uint8_t r = (config.bg_color >> 24) & 0xFF;
    const uint8_t g = (config.bg_color >> 16) & 0xFF;
    const uint8_t b = (config.bg_color >> 8) & 0xFF;
    const uint8_t a = config.bg_color & 0xFF;

    SDL_SetRenderDrawColor(sdl->renderer, r, g, b, a);
    SDL_RenderClear(sdl->renderer);

}

void update_screen(sdl_t *sdl){
    SDL_RenderPresent(sdl->renderer);
}

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    config_t config = { 0 };
    if (!set_config_from_args(&config, argc, argv))
        exit(EXIT_FAILURE);

    sdl_t sdl = { 0 };
    if (!init_sdl(&sdl, &config))
        exit(EXIT_FAILURE);

    clear_screen(config, &sdl);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) 
                quit = true;

            if(event.type == SDL_KEYDOWN)
                quit = true;

        
        SDL_Delay(16);
        update_screen(&sdl);
    }

    // Perform any other game logic or updates here
}


    final_cleanup(&sdl);

    puts("Testing on windows \n");
    exit(EXIT_SUCCESS);
}