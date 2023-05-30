#include <string>
#include "SDL.h"

class Emulator{
    public:
        bool CHIP8emulator(const std::string &filename);
        void InitBeep();
        void Beep();
        void Quit();

    private:
        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8 *wavBuffer;
        SDL_AudioDeviceID deviceId;

        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL; 
        SDL_Texture *texture = NULL;
};