#include "SDL.h"
#include <iostream>

#define fps 60
#define W_width 640
#define W_height 820

using namespace std;

void cap_framerate(Uint32 start_tick){
    if((1000 / fps) > SDL_GetTicks() - start_tick)
        SDL_Delay(1000 / fps - (SDL_GetTicks() - start_tick));
}

class Sprite{
    private:
        SDL_Surface* image;
        SDL_Rect rect;

        int origin_x, origin_y;
    public:
        Sprite(Uint32 color, int x, int y, int w = 48, int h = 64){
            image = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
            SDL_FillRect(image, NULL, color);

            rect = image->clip_rect;

            origin_x, origin_y = 0;

            rect.x = x - origin_x;
            rect.y = y - origin_y;
        }

        void update(){

        }

        void draw(SDL_Surface *destination){
            SDL_BlitSurface(image, NULL, destination, &rect);
        }

};

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = NULL;

    window = SDL_CreateWindow(
        "window",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED, 
        W_width, W_height,
        SDL_WINDOW_RESIZABLE
    );

    SDL_Surface *surface = SDL_GetWindowSurface(window); 

    Uint32 white = SDL_MapRGB(surface -> format, 255, 255, 255);
    Uint32 red = SDL_MapRGB(surface -> format, 255, 0, 0);

    SDL_FillRect(surface, NULL, white);

    Sprite object(red, W_width / 2, W_height / 2);
    object.draw(surface);

    SDL_UpdateWindowSurface(window);

    Uint32 start_tick;

    SDL_Event event;
    bool quit = false;

    while(!quit){

        start_tick = SDL_GetTicks();

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                quit = true;

            if(event.type == SDL_KEYDOWN)
                quit = true;
        }

        cap_framerate(start_tick);
    }

    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}