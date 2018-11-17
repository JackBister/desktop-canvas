#pragma once

#include "../../duktape.h"

struct SDL_Renderer;

namespace dcanvas {
	void init_bitmap(duk_context * ctx, SDL_Renderer * renderer);
}
