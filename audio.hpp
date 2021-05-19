#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL.h>
#include <iostream>

class Audio
{
public:
    ~Audio();
    void load(const char *filename);
    void play();
    void stop();

private:
    SDL_AudioSpec wavSpec;
    Uint32 wavLength;
    Uint8 *wavBuffer;
    SDL_AudioDeviceID deviceId;
};

#endif //AUDIO_H
