#include <SDL2/SDL.h>
#include <string>
#include "MusicPlayer.h"

void MusicPlayer::Play(std::string filename)
{
	SDL_Init(SDL_INIT_AUDIO);

	// Load in the values of wavSpec, wavBuffer, and wavLength
	SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength);

	deviceID = SDL_OpenAudioDevice(
		nullptr,  // Device
		0,        // Is capture
		&wavSpec, // Audio spec
		nullptr,  // Audio Spec obtained, 
		0         // Allowed changes
	);

	// Queue the audio for play back
	SDL_QueueAudio(deviceID, wavBuffer, wavLength);
	SDL_PauseAudioDevice(deviceID, 0);    // Start playing the audio

	// If all the audio has been played wait 100 miliseconds
	while (SDL_GetQueuedAudioSize(deviceID) > 0)
		SDL_Delay(100);

	if (deviceID != 0) {    // If an audio device has been connected
		SDL_FreeWAV(wavBuffer);
		SDL_CloseAudioDevice(deviceID);
	}

	SDL_Quit();
}