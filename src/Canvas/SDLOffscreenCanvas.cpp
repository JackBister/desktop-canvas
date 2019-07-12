#include "SDLOffscreenCanvas.h"

#include <SDL2/SDL.h>

#include "./SDLCanvasRenderingContext2D.h"

SDLOffscreenCanvas::SDLOffscreenCanvas(SDL_Renderer * renderer, int width, int height)
    : renderer(renderer), width(width), height(height)
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

void SDLOffscreenCanvas::setHeight(int height)
{
    Uint32 format;
    int access, width;
    SDL_QueryTexture(texture, &format, &access, &width, nullptr);
    auto oldTexture = texture;
    texture = SDL_CreateTexture(renderer, format, access, width, height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, oldTexture, nullptr, nullptr);
    SDL_DestroyTexture(oldTexture);
}

int SDLOffscreenCanvas::getWidth()
{
    int width;
    auto result = SDL_QueryTexture(texture, nullptr, nullptr, &width, nullptr);
    // TODO: handle errors
    return width;
}

void SDLOffscreenCanvas::setWidth(int width)
{
    Uint32 format;
    int access, height;
    SDL_QueryTexture(texture, &format, &access, nullptr, &height);
    auto oldTexture = texture;
    texture = SDL_CreateTexture(renderer, format, access, width, height);
    SDL_SetRenderTarget(renderer, texture);
    SDL_RenderCopy(renderer, oldTexture, nullptr, nullptr);
    SDL_DestroyTexture(oldTexture);
}

CanvasRenderingContext2D * SDLOffscreenCanvas::getContext()
{
    return context;
}

std::unique_ptr<SDLBitmap> SDLOffscreenCanvas::asBitmap()
{
    return std::unique_ptr<SDLBitmap>(new SDLBitmap(texture, height, width));
}
