
#include <optional>

#include <android/log.h>

#include <SDL2/SDL_config_android.h>
#include <SDL2/SDL.h>

#include "DukJavaScriptEngine.h"
#include "JavaScriptEngine.h"
#include "slurp_file.h"
#include "watch_file.h"
#include "Canvas/SDLCanvasRenderingContext2D.h"

std::unique_ptr<IJavaScriptEngine> g_jsEngine;

int main(int argc, char *argv[]) {
	auto window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 256, 144, SDL_WINDOW_FULLSCREEN_DESKTOP);
	if (!window) {
		__android_log_print(ANDROID_LOG_INFO, "dcanvas", "window NULL! %s", SDL_GetError());
		return 1;
	}

	auto surface = SDL_GetWindowSurface(window);
	if (!surface) {
		__android_log_print(ANDROID_LOG_INFO, "dcanvas", "surface NULL! %s", SDL_GetError());
		return 1;
	}

	auto renderer = SDL_GetRenderer(window);
	if (!renderer) {
		__android_log_print(ANDROID_LOG_INFO, "dcanvas", "renderer NULL! %s, trying CreateRenderer...", SDL_GetError());
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (!renderer) {
			__android_log_print(ANDROID_LOG_INFO, "dcanvas", "renderer still NULL! %s, trying CreateSoftwareRenderer...", SDL_GetError());
			renderer = SDL_CreateSoftwareRenderer(surface);
			if (!renderer) {
				__android_log_print(ANDROID_LOG_INFO, "dcanvas", "renderer still NULL! %s, giving up...", SDL_GetError());
				return 1;
			}
		}
	}

	ICanvasRenderingContext2D * canvas = new SDLCanvasRenderingContext2D(renderer, surface);

	auto backbuffer = SDL_CreateTexture(renderer, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 320, 180);
	SDL_SetRenderTarget(renderer, backbuffer);

	if (SDL_RenderSetLogicalSize(renderer, 400, 225)) {
		__android_log_print(ANDROID_LOG_INFO, "dcanvas", "RenderSetLogicalSize failed, %s", SDL_GetError());
	}

	g_jsEngine = std::make_unique<DukJavaScriptEngine>();
	g_jsEngine->eval_file("app.js");
	g_jsEngine->init_bitmap(canvas);
	g_jsEngine->init_websocket();
	g_jsEngine->init_canvas(canvas);


	for (;;) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				goto end;
			}
		}

		SDL_SetRenderTarget(renderer, backbuffer);
		g_jsEngine->pre_tick();
		g_jsEngine->call_global_function("tick");

		SDL_SetRenderTarget(renderer, nullptr);
		SDL_RenderCopy(renderer, backbuffer, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

end:
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	return 0;
}
