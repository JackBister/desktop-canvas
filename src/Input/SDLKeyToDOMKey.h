#pragma once

#include <optional>
#include <string>

// Workaround for NDK retardation
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

namespace dcanvas {
	std::optional<std::string> SDLKeyToDOMKey(SDL_Keycode keycode);
}
