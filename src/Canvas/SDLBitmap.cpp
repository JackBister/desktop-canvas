#include "SDLBitmap.h"

#ifdef __ANDROID__
#include <SDL2/SDL_config_android.h>
#endif
#include <SDL2/SDL.h>

SDLBitmap::SDLBitmap(SDL_Texture * texture, int height, int width)
    : texture(texture), height(height), width(width)
{
}

int SDLBitmap::getHeight()
{
    return height;
}

int SDLBitmap::getWidth()
{
    return width;
}

void SDLBitmap::close()
{
    SDL_DestroyTexture(texture);
}

SDL_Texture * SDLBitmap::getTexture()
{
    return texture;
}
