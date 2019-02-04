#pragma once

#include <SDL2/SDL.h>

#include "Gamepad.h"

class SDLGamepad : public Gamepad
{
public:
  SDLGamepad(SDL_JoystickID, SDL_GameController *);

  virtual std::array<double, 4> getAxes() override;
  virtual std::array<GamepadButton, 16> getButtons() override;
  virtual std::string getId() override;

  SDL_JoystickID getJoystickId();

private:
  GamepadButton getButton(SDL_GameControllerButton button);
  GamepadButton getTrigger(SDL_GameControllerAxis trigger);

  SDL_GameController * gameController;
  SDL_JoystickID joystickId;
};
