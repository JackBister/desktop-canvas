#pragma once

// Workaround for NDK retardation
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "../../duktape.h"

namespace dcanvas {
	void init_canvas(duk_context * ctx, SDL_Renderer * renderer, SDL_Surface * surface);
}
