#pragma once
#include "CanvasRenderingContext2D.h"

// Workaround for NDK retardation
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "SDLBitmap.h"

class SDLCanvasRenderingContext2D : public ICanvasRenderingContext2D {
public:
	SDLCanvasRenderingContext2D(SDL_Renderer * renderer, SDL_Surface * surface);

	virtual SDLBitmap * createBitmap(void * data, size_t size) override;

	virtual void drawImage(Bitmap * image, int dx, int dy) override;
	virtual void drawImage(Bitmap * image, int dx, int dy, int dWidth, int dHeight) override;
	virtual void drawImage(Bitmap * image, int sx, int sy, int sWidth, int sHeight, int dx, int dy, int dWidth, int dHeight) override;

	virtual void fillRect(int x, int y, int width, int height) override;

	virtual std::string const& getFillStyle() override { return fillStyle; }
	virtual void setFillStyle(std::string const& val) override { fillStyle = val; }


private:
	SDL_Renderer * renderer;
	SDL_Surface * surface;

	std::string fillStyle = "#000000";
	std::string strokeStyle = "#000000";
};
