#pragma once

#include <optional>
#include <string>

#ifdef __ANDROID__
#include <SDL2/SDL_config_android.h>
#endif
#include <SDL2/SDL.h>

namespace dcanvas {
	std::optional<std::string> SDLKeyToDOMKey(SDL_Keycode keycode);
}
