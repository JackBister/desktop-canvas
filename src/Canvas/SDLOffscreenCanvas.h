#pragma once

#include <memory>

#include "./Canvas.h"
#include "./SDLBitmap.h"

struct SDL_Renderer;
struct SDL_Texture;

class SDLOffscreenCanvas : public Canvas
{
  public:
    SDLOffscreenCanvas(SDL_Renderer * renderer, int width, int height);

    virtual int getHeight() override;
    virtual int getWidth() override;

    virtual CanvasRenderingContext2D * getContext() override;

    std::unique_ptr<SDLBitmap> asBitmap();

  private:
    int width, height;
    CanvasRenderingContext2D * context;
    SDL_Texture * texture;
};
