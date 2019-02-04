#include "SDLNavigator.h"

#include <SDL2/SDL.h>

#include "../Logger/Logger.h"

static auto logger = Logger::get();

SDLNavigator::SDLNavigator()
{
	SDL_GameControllerEventState(SDL_ENABLE);
	for (int i = 0; i < SDL_NumJoysticks(); ++i) {
		if (SDL_IsGameController(i)) {
			auto gameController = SDL_GameControllerOpen(i);
			if (!gameController) {
				logger->info("ERROR: Joystick idx %d was identified as a gamepad but SDL_GameControllerOpen failed", i);
			}
			auto instanceId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
			auto mapping = SDL_GameControllerMapping(gameController);
			logger->info("mapping %s", mapping);
			gamepads.emplace_back(instanceId, gameController);
		}
	}
	SDL_AddEventWatch(SDLNavigator::sdlEventWatch, this);
}

SDLNavigator::~SDLNavigator()
{
	SDL_DelEventWatch(SDLNavigator::sdlEventWatch, this);
}

std::array<Gamepad *, 4> SDLNavigator::getGamepads()
{
	return std::array<Gamepad *, 4>({
		gamepads.size() > 0 ? &gamepads[0] : nullptr,
		gamepads.size() > 1 ? &gamepads[1] : nullptr,
		gamepads.size() > 2 ? &gamepads[2] : nullptr,
		gamepads.size() > 3 ? &gamepads[3] : nullptr,
	});
}

int SDLNavigator::sdlEventWatch(void * userdata, SDL_Event * evt)
{
	if (evt->type == SDL_JOYBUTTONDOWN || evt->type == SDL_CONTROLLERBUTTONDOWN) {
		return 0;
	}
	if (evt->type != SDL_CONTROLLERDEVICEADDED && evt->type != SDL_CONTROLLERDEVICEREMOVED) {
		return 0;
	}

	SDLNavigator * nav = (SDLNavigator *)userdata;

	if (evt->type == SDL_CONTROLLERDEVICEADDED) {
		auto gameController = SDL_GameControllerOpen(evt->cdevice.which);
		if (!gameController) {
			logger->info("ERROR: Joystick idx %d was identified as a gamepad but SDL_GameControllerOpen failed", evt->cdevice.which);
		}
		auto instanceId = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(gameController));
		nav->gamepads.push_back(SDLGamepad(instanceId, gameController));
	} else if (evt->type == SDL_CONTROLLERDEVICEREMOVED) {
		auto instanceId = evt->cdevice.which;
		auto removeMe = std::find_if(nav->gamepads.begin(), nav->gamepads.end(), [instanceId](SDLGamepad& gamepad) {
			return gamepad.getJoystickId() == instanceId;
		});
		nav->gamepads.erase(removeMe);
	}

	return 0;
}
