#include <iostream>
#include <stdint.h>
#include <cstring>
#include <string>

#include "SDL.h"
#include "CHIP8.h"
#include "emulator.h"

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define GRAPHICS_HEIGHT 32
#define GRAPHICS_WIDTH 64

#define CLOCK_DELAYER 0
#define TOTAL_KEYS 16

#define FPS_CAP 60

using namespace std;

unsigned char SDLKeys[TOTAL_KEYS] = {
    SDLK_x, 
    SDLK_1, 
    SDLK_2, 
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
};


void Emulator::InitBeep(){
    SDL_LoadWAV("Beep.wav", &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);
}

void Emulator::Beep(){
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);

    while(SDL_GetQueuedAudioSize(deviceId) > 0)
        SDL_Delay(500);
}

void Emulator::Quit(){
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
    SDL_Quit();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyTexture(texture);

    SDL_Quit();
}

bool Emulator::CHIP8emulator(const string &filename){
    CHIP8 chip8;

    chip8.initialize();
    chip8.LoadROM(filename);

    InitBeep();

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "StarPlayer",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT, 
        SDL_WINDOW_RESIZABLE
    );

    if(window == NULL){
        cerr << "Error in loading window \n" << SDL_GetError() << endl;
        exit(0);
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(renderer == NULL){
        cerr << "Could not load renderer \n" << SDL_GetError() << endl;
        exit(0);
    }

    texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        GRAPHICS_WIDTH, 
        GRAPHICS_HEIGHT
    );

    unsigned int pixels[GRAPHICS_WIDTH * GRAPHICS_HEIGHT];
    int timer = 0;
    int start = SDL_GetTicks();
    int start_per_frame = SDL_GetTicks();
    long countedFrames = 0;

    while(true){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                Quit();
                return true;
            }
            
            if(event.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_ESCAPE){
                    Quit();
                    return true;
                }

                for (int i = 0; i < TOTAL_KEYS; ++i){
                    if(event.key.keysym.sym == SDLKeys[i])
                        chip8.key[i] = 1;
                }
            }

            if(event.type == SDL_KEYUP){
                for(int i = 0; i < TOTAL_KEYS; ++i){
                    if(event.key.keysym.sym == SDLKeys[i])
                        chip8.key[i] = 0;
                }
            }
            
        }

        if(timer == CLOCK_DELAYER){
            chip8.emulateCycle();
            timer = 0;
        }else
            timer += 1;

        int pitch;
        void *m_pixels;

        if(chip8.drawFlag){
            if(SDL_LockTexture(texture, NULL, (void **)&m_pixels, &pitch) > 0)
                cout << endl;

            for(int i = 0; i < (GRAPHICS_WIDTH * GRAPHICS_HEIGHT); i++)
                pixels[i] = ((0x00FFFFFF * chip8.graphics[i]) | 0xFF000000);
            
            memcpy(m_pixels, pixels, GRAPHICS_HEIGHT * pitch);
            SDL_UnlockTexture(texture);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            chip8.drawFlag = false;

            if((1000 / FPS_CAP) > (SDL_GetTicks() - start_per_frame))
                SDL_Delay((1000 / FPS_CAP) - (SDL_GetTicks() - start_per_frame));

            start_per_frame = SDL_GetTicks();

            float avgFps = countedFrames / ((SDL_GetTicks() - start) / 1000.f);
            if(avgFps > 2000000)
                avgFps = 0;

            ++countedFrames;
            ///cerr << "Average FPS: " << avgFps << "\r";
        }      

        if(chip8.beepFlag){
            Beep();
            chip8.beepFlag = false;
        }
    }

    Quit();
}