#include "duk_canvas.h"

#include <string>

// Workaround for NDK retardation
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "../../duktape.h"
#include "../../../Logger/Logger.h"

#include "CanvasState.h"
#include "drawImage.h"

static void defineProp(duk_context * ctx, std::string const& name, int(*getter)(duk_context *), int(*setter)(duk_context *)) {
	duk_push_string(ctx, name.c_str());
	duk_push_c_function(ctx, getter, 0);
	duk_push_c_function(ctx, setter, 1);
	duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
}

static void defineMethod(duk_context * ctx, std::string const& name, int(*method)(duk_context *), int nargs) {
	duk_push_string(ctx, name.c_str());
	duk_push_c_function(ctx, method, nargs);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);
}


static int fillRect(duk_context * ctx) {
	auto state = CanvasState::fromThis(ctx);

	auto x = duk_require_int(ctx, -4);
	auto y = duk_require_int(ctx, -3);
	auto width = duk_require_int(ctx, -2);
	auto height = duk_require_int(ctx, -1);

	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	auto color = state->getFillColor();

	SDL_SetRenderDrawColor(state->getRenderer(), color.r, color.g, color.b, 0xFF);
	SDL_RenderFillRect(state->getRenderer(), &rect);

	return 0;
}

static int getFillStyle(duk_context * ctx) {
	auto state = CanvasState::fromThis(ctx);

	duk_push_string(ctx, state->getFillStyle().c_str());
	return 1;
}

static int setFillStyle(duk_context * ctx) {
	auto state = CanvasState::fromThis(ctx);

	state->setFillStyle(std::string(duk_require_string(ctx, -1)));
	return 1;
}

static int finalizeCanvas(duk_context * ctx) {
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");
	CanvasState * state = (CanvasState *)duk_get_pointer(ctx, -1);

	delete state;

	duk_pop(ctx);
	return 0;
}

namespace dcanvas {
	auto logger = ILogger::get();

	void init_canvas(duk_context * ctx, SDL_Renderer * renderer, SDL_Surface * surface) {
		auto state = new CanvasState(renderer, surface);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);

		duk_get_global_string(ctx, "init");

		// Create an object with the property __internalPtr which is a pointer to the CanvasState
		// Apparently you can not define properties on a pointer as you could with a light userdata in Lua?
		duk_push_object(ctx);
		duk_push_string(ctx, "\xFF" "\xFF" "internalPtr");
		duk_push_pointer(ctx, state);
		duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

		defineProp(ctx, "fillStyle", getFillStyle, setFillStyle);

		defineMethod(ctx, "fillRect", fillRect, 4);
		defineMethod(ctx, "drawImage", dcanvas::drawImage, DUK_VARARGS);

		duk_push_c_function(ctx, finalizeCanvas, 1);
		duk_set_finalizer(ctx, -2);

		duk_pcall(ctx, 1);
		logger->info("init call finished");
	}
}
