#pragma once

#include "EventPump.h"

class SDLEventPump : public EventPump
{
public:
  SDLEventPump(std::pair<int, int> windowSize);

  virtual bool pumpEvents(IJavaScriptEngine * into) override;

private:
  std::pair<int, int> windowSize;
};
