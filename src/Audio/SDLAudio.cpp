#include "SDLAudio.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../Logger/Logger.h"

static auto logger = Logger::get();

SDLAudio::SDLAudio(std::string const & src)
{
    // TODO: Async
    chunk = Mix_LoadWAV(src.c_str());
}

void SDLAudio::pause()
{
    if (channel == -1) {
        // -1 is used as a sentinel value for an Audio that hasn't been play()ed yet
        return;
    }
    // TODO: This doesn't really work
    Mix_Pause(channel);
}

void SDLAudio::play()
{
    if (channel == -1) {
        channel = Mix_PlayChannel(-1, chunk, 0);
    } else {
        Mix_PlayChannel(channel, chunk, 0);
    }
}
