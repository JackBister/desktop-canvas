#include "SDLEventPump.h"

#include <SDL2/SDL.h>

#include "Input/SDLKeyToDOMKey.h"
#include "Logger/Logger.h"

static auto logger = Logger::get();

static std::string getTouchEventType(Uint32 t) {
	switch (t) {
	case SDL_FINGERDOWN:
		return "touchstart";
	case SDL_FINGERUP:
		return "touchend";
	default:
		logger->info("Unhandled EventType in getTouchEventType %d", t);
		return "";
	}
}

SDLEventPump::SDLEventPump(std::pair<int, int> windowSize) : windowSize(windowSize) { }

bool SDLEventPump::pumpEvents(IJavaScriptEngine * into)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return true;
		}
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
			auto key = e.key;
			auto codeOptional = dcanvas::SDLKeyToDOMKey(key.keysym.sym);
			if (codeOptional.has_value()) {
				JSObject keyboardEvent = {
					{ "altKey", (key.keysym.mod & KMOD_ALT) != 0 },
					{ "code", codeOptional.value() },
					{ "ctrlKey", (key.keysym.mod & KMOD_CTRL) != 0 },
					{ "metaKey", (key.keysym.mod & KMOD_GUI) != 0 },
					{ "repeat", key.repeat != 0 },
					{ "shiftKey", (key.keysym.mod & KMOD_SHIFT) != 0 }
				};
				if (e.type == SDL_KEYDOWN) {
					into->callGlobalFunction("onkeydown", keyboardEvent);
				} else {
					into->callGlobalFunction("onkeyup", keyboardEvent);
				}
			}
		} else if (e.type == SDL_MOUSEBUTTONDOWN) {
			auto mouse = e.button;
			JSObject mouseEvent = {
				{ "button", (double)(mouse.button - 1) },
				{ "clientX", (double)mouse.x },
				{ "clientY", (double)mouse.y }
			};
			into->callGlobalFunction("onmousedown", mouseEvent);
		} else if (e.type == SDL_FINGERDOWN || e.type == SDL_FINGERUP) {
			auto finger = e.tfinger;

			// finger.x/finger.y are 0..1 normalized, need to convert to screen coordinates
			auto clientX = finger.x * windowSize.first;
			auto clientY = finger.y * windowSize.second;

			JSObject touch = {
				{ "clientX", (double)clientX },
				{ "clientY", (double)clientY },
				{ "force", finger.pressure }
			};

			JSArray changedTouches = { touch };

			JSObject touchEvent = {
				{ "type", getTouchEventType(e.type) },
				{ "changedTouches", changedTouches }
			};

			if (e.type == SDL_FINGERDOWN) {
				into->callGlobalFunction("ontouchstart", touchEvent);
			} else {
				into->callGlobalFunction("ontouchend", touchEvent);
			}
		}
	}
	return false;
}
