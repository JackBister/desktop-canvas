#pragma once

#include <string>

#include <SDL2/SDL.h>

#include "../../duktape.h"

struct Color {
	Uint8 r;
	Uint8 g;
	Uint8 b;
};

struct CanvasState {
	static CanvasState * fromThis(duk_context * ctx);

	CanvasState(SDL_Renderer * renderer, SDL_Surface * surface) : renderer(renderer), surface(surface) {}

	void drawImage(SDL_Texture * image, int dx, int dy);
	void drawImage(SDL_Texture * image, int dx, int dy, int dWidth, int dHeight);
	void drawImage(SDL_Texture * image, int sx, int sy, int sWidth, int sHeight, int dx, int dy, int dWidth, int dHeight);

	std::string const& getFillStyle() { return fillStyle; }
	void setFillStyle(std::string const& val) { fillStyle = val; }

	Color getFillColor();

	SDL_Renderer * getRenderer() { return renderer; }
	SDL_Surface * getSurface() { return surface; }

private:
	SDL_Renderer * renderer;
	SDL_Surface * surface;

	std::string fillStyle = "#000000";
	std::string strokeStyle = "#000000";

};