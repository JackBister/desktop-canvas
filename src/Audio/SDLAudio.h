#pragma once

#include "Audio.h"

#include <SDL2/SDL_mixer.h>

class SDLAudio : public Audio
{
  public:
    SDLAudio(std::string const & src);

    virtual void pause() override;
    virtual void play() override;

  private:
    int channel = -1;
    Mix_Chunk * chunk;
};
