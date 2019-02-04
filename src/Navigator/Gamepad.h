#pragma once

#include <array>

class GamepadButton
{
  public:
    GamepadButton(bool pressed, double value) : pressed(pressed), value(value) {}

    bool getPressed() { return pressed; }
    double getValue() { return value; }

  private:
    bool pressed;
    double value;
};

class Gamepad
{
  public:
    virtual ~Gamepad();

    virtual std::array<double, 4> getAxes() = 0;
    virtual std::array<GamepadButton, 16> getButtons() = 0;
    virtual std::string getId() = 0;
};
