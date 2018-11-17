#pragma once

#include <optional>
#include <string>

#include <SDL2/SDL.h>

namespace dcanvas {
	std::optional<std::string> SDLKeyToDOMKey(SDL_Keycode keycode);
}
