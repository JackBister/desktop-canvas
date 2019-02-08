#pragma once

#include <cstdint>
#include <memory>
#include <vector>

class Audio
{
  public:
    virtual ~Audio();

    virtual void pause() = 0;
    virtual void play() = 0;
};
