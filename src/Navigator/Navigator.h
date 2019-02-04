#pragma once

#include <array>

#include "Gamepad.h"

class Navigator
{
public:
  virtual ~Navigator();

  virtual std::array<Gamepad *, 4> getGamepads() = 0;
};
