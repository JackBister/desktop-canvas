#include "SDLEventPump.h"

#include <SDL2/SDL.h>

#include "Input/SDLKeyToDOMKey.h"

void SDLEventPump::pumpEvents(IJavaScriptEngine * into)
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			// TODO: 
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
		}
	}
}
