#include "SDL.h"
#include <iostream>
#include <cstring>

using namespace std;

class playMusic{
    private:
        SDL_AudioSpec wavSpec;
        Uint32 wavLength;
        Uint8* wavBuffer;

        SDL_AudioDeviceID deviceId;

    public:
        void play(char* name){
            SDL_LoadWAV(name, &wavSpec, &wavBuffer, &wavLength);
            deviceId = SDL_OpenAudioDevice(nullptr, 0, &wavSpec, nullptr, 0);

            SDL_QueueAudio(deviceId, wavBuffer, wavLength);
            SDL_PauseAudioDevice(deviceId, 0);

            while(SDL_GetQueuedAudioSize(deviceId) > 0)
                SDL_Delay(100);
        }

        void quit(){
            if (deviceId != 0){
                SDL_FreeWAV(wavBuffer);
                SDL_CloseAudioDevice(deviceId);
            }
        }
};

int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_AUDIO);
    playMusic playmusic;

    while(true){
        char* music = new char[256];

        cout << "Enter the path to your music file here: ";
        cin >> music;

        if (strcmp(music, "exit") == 0){
            playmusic.quit();
            delete[] music;
            break;
        }

        playmusic.play(music);
        delete[] music;
    }

    SDL_Quit();
    return 0;
}