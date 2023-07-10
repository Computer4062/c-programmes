#include "SDL.h"
#include <iostream>
#include <process.h>

using namespace std;

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Surface* image = SDL_LoadBMP(argv[1]);

    SDL_Window* window = SDL_CreateWindow(
        argv[1],
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        image -> w, image -> h, 
        SDL_WINDOW_RESIZABLE
    );

    if(image == NULL){
        cout << "Error in loading file" << endl;
        cout << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
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
