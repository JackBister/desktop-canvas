#include "duk_bitmap.h"

#include <string>

// Workaround for NDK retardation
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "../../../Logger/Logger.h"

static auto logger = ILogger::get();

static SDL_Renderer * renderer = nullptr;

static int finalizeBitmap(duk_context * ctx) {
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");
	SDL_Texture * texture = (SDL_Texture *)duk_get_pointer(ctx, -1);

	SDL_DestroyTexture(texture);

	duk_pop(ctx);
	return 0;
}

static int closeBitmap(duk_context * ctx) {
	duk_push_this(ctx);
	auto ret = finalizeBitmap(ctx);
	duk_pop(ctx);
	return ret;
}

static int loadBitmap(duk_context * ctx) {
	logger->info("loadBitmap");
	std::string filename(duk_require_string(ctx, -2));
	duk_require_function(ctx, -1);

	auto surface = SDL_LoadBMP(filename.c_str());
	logger->info("bmp loaded");
	if (!surface) {
		logger->info("bmp surface NULL!");
	}

	auto texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (!texture) {
		logger->info("texture NULL! %s", SDL_GetError());
	}
	logger->info("texture not NULL! %s", "test");

	duk_push_object(ctx);

	duk_push_pointer(ctx, texture);
	duk_put_prop_string(ctx, -2, "\xFF" "\xFF" "internalPtr");

	duk_push_string(ctx, "height");
	duk_push_number(ctx, surface->h);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);

	duk_push_string(ctx, "width");
	duk_push_number(ctx, surface->w);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);

	duk_push_c_function(ctx, closeBitmap, 0);
	duk_put_prop_string(ctx, -2, "close");

	duk_push_c_function(ctx, finalizeBitmap, 1);
	duk_set_finalizer(ctx, -2);

	// Call the callback the user passed in with our newly constructed object as the argument
	duk_pcall(ctx, 1);

	duk_pop(ctx);

	return 0;
}

namespace dcanvas {
	void init_bitmap(duk_context * ctx, SDL_Renderer * rend)
	{
		renderer = rend;

		duk_push_global_object(ctx);
		duk_push_c_function(ctx, loadBitmap, 2);
		duk_put_prop_string(ctx, -2, "loadBitmap");
		duk_pop(ctx);
	}
}
