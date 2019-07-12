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

void SDLWindowCanvas::setHeight(int height)
{
    int width, oldHeight;
    SDL_GetWindowSize(window, &width, &oldHeight);
    SDL_SetWindowSize(window, width, height);
}

int SDLWindowCanvas::getWidth()
{
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    return w;
}

void SDLWindowCanvas::setWidth(int width)
{
    int oldWidth, height;
    SDL_GetWindowSize(window, &oldWidth, &height);
    SDL_SetWindowSize(window, width, height);
}

CanvasRenderingContext2D * SDLWindowCanvas::getContext()
{
    return context;
}
