#pragma once

#include "EventPump.h"

class SDLEventPump : public EventPump
{
  public:
    SDLEventPump(std::pair<int, int> windowSize);

    virtual bool pumpEvents(JavaScriptEngine * into) override;

  private:
    std::pair<int, int> windowSize;
};
