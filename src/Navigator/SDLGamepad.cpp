#include "SDLGamepad.h"

#include <cstdint>
#include <stdexcept>

#include "../Logger/Logger.h"

static auto logger = Logger::get();

static double normalizeInt16(int16_t n)
{
    return std::max(n / (double)INT16_MAX, -1.0);
}

SDLGamepad::SDLGamepad(SDL_JoystickID joystickId, SDL_GameController * gameController)
    : joystickId(joystickId), gameController(gameController)
{
}

std::array<double, 4> SDLGamepad::getAxes()
{
    auto lx = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTX);
    auto ly = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_LEFTY);
    auto rx = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTX);
    auto ry = SDL_GameControllerGetAxis(gameController, SDL_CONTROLLER_AXIS_RIGHTY);
    return std::array<double, 4>(
        {normalizeInt16(lx), normalizeInt16(ly), normalizeInt16(rx), normalizeInt16(ry)});
}

std::array<GamepadButton, 16> SDLGamepad::getButtons()
{
    return std::array<GamepadButton, 16>(
        {getButton(SDL_CONTROLLER_BUTTON_A), getButton(SDL_CONTROLLER_BUTTON_B),
         getButton(SDL_CONTROLLER_BUTTON_X), getButton(SDL_CONTROLLER_BUTTON_Y),
         getButton(SDL_CONTROLLER_BUTTON_LEFTSHOULDER),
         getButton(SDL_CONTROLLER_BUTTON_RIGHTSHOULDER),
         getTrigger(SDL_CONTROLLER_AXIS_TRIGGERLEFT), getTrigger(SDL_CONTROLLER_AXIS_TRIGGERRIGHT),
         getButton(SDL_CONTROLLER_BUTTON_BACK), getButton(SDL_CONTROLLER_BUTTON_START),
         getButton(SDL_CONTROLLER_BUTTON_LEFTSTICK), getButton(SDL_CONTROLLER_BUTTON_RIGHTSTICK),
         getButton(SDL_CONTROLLER_BUTTON_DPAD_UP), getButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN),
         getButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT), getButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT)});
}

std::string SDLGamepad::getId()
{
    return SDL_GameControllerName(gameController);
}

SDL_JoystickID SDLGamepad::getJoystickId()
{
    return joystickId;
}

GamepadButton SDLGamepad::getButton(SDL_GameControllerButton button)
{
    auto btn = SDL_GameControllerGetButton(gameController, button);
    return GamepadButton(btn != 0, btn != 0 ? 1.0 : 0.0);
}

GamepadButton SDLGamepad::getTrigger(SDL_GameControllerAxis trigger)
{
    if (trigger != SDL_CONTROLLER_AXIS_TRIGGERLEFT && trigger != SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
        logger->info("ERROR: Invalid input for getTrigger %d", trigger);
        throw std::invalid_argument("Invalid input for getTrigger");
    }

    auto axis = SDL_GameControllerGetAxis(gameController, trigger);
    return GamepadButton(axis != 0, normalizeInt16(axis));
}
