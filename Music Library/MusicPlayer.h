#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <SDL2/SDL.h>
#include <string>

class MusicPlayer {
public:
	void Play(std::string filename);

private:
	SDL_AudioSpec wavSpec;      // Audio settings
	Uint32 wavLength;           // speed / frequency
	Uint8* wavBuffer;           // Audio data
	SDL_AudioDeviceID deviceID; // Speaker ID
};

#endif