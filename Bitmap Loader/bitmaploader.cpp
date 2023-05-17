#include "SDL.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);

    const char filename[] = "image.bmp";

    SDL_Window* window = SDL_CreateWindow(
        "image",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1000, 1000,
        SDL_WINDOW_RESIZABLE
    );

    SDL_Surface *image = SDL_LoadBMP(filename);

    if (image == NULL){
        cout << "image could not be loaded" << endl << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Surface* screen = SDL_GetWindowSurface(window);
    SDL_BlitSurface(image, NULL, screen, NULL);
    SDL_UpdateWindowSurface(window);

    SDL_Event event;
    bool quit = false;

    while(!quit){
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                quit = true;
            
            if(event.type == SDL_KEYDOWN)
                quit = true;
        }
    }

    SDL_FreeSurface(image);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}