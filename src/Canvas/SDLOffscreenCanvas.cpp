#include "SDLOffscreenCanvas.h"

#include <SDL2/SDL.h>

#include "./SDLCanvasRenderingContext2D.h"

SDLOffscreenCanvas::SDLOffscreenCanvas(SDL_Renderer * renderer, int width, int height)
    : width(width), height(height)
{
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width,
                                height);
    context = new SDLCanvasRenderingContext2D(this, renderer, texture);
}

int SDLOffscreenCanvas::getHeight()
{
    int height;
    auto result = SDL_QueryTexture(texture, nullptr, nullptr, nullptr, &height);
    // TODO: handle errors
    return height;
}

int SDLOffscreenCanvas::getWidth()
{
    int width;
    auto result = SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
    // TODO: handle errors
    return width;
}

CanvasRenderingContext2D * SDLOffscreenCanvas::getContext()
{
    return context;
}

std::unique_ptr<SDLBitmap> SDLOffscreenCanvas::asBitmap()
{
    return std::unique_ptr<SDLBitmap>(new SDLBitmap(texture, height, width));
}
