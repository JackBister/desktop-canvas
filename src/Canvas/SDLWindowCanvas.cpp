#include "SDLWindowCanvas.h"

#include <stdexcept>

#include <SDL2/SDL.h>

#include "./SDLCanvasRenderingContext2D.h"

SDLWindowCanvas::SDLWindowCanvas(SDL_Window * window, SDL_Renderer * renderer) : window(window)
{
    context = new SDLCanvasRenderingContext2D(this, renderer, nullptr);
}

int SDLWindowCanvas::getHeight()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return h;
}

int SDLWindowCanvas::getWidth()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return w;
}

CanvasRenderingContext2D * SDLWindowCanvas::getContext()
{
    return context;
}
