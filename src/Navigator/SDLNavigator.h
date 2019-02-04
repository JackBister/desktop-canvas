#pragma once

#include <vector>

#include "Navigator.h"
#include "SDLGamepad.h"

class SDLNavigator : public Navigator
{
public:
  SDLNavigator();
  ~SDLNavigator();

  virtual std::array<Gamepad *, 4> getGamepads() override;

private:
  static int sdlEventWatch(void * userdata, SDL_Event * evt);

  std::vector<SDLGamepad> gamepads;
};
