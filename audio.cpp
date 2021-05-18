#include "audio.hpp"

Audio::~Audio() {
    SDL_CloseAudioDevice(deviceId);
    SDL_FreeWAV(wavBuffer);
}

void Audio::load(const char* filename) {
    std::cout<<"will play music lodaed "<<filename<< "\n";
    SDL_LoadWAV(filename, &wavSpec, &wavBuffer, &wavLength);
    deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
}

void Audio::play() {
    // std::cout<<"will play music \n";
    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, 0);
}

void play_music(const char* filename){
    Audio effect;
    effect.load(filename);
    effect.play();
}

