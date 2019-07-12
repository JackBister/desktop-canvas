#pragma once

#include "./Canvas.h"

struct SDL_Renderer;
struct SDL_Window;

class SDLWindowCanvas : public Canvas
{
  public:
    SDLWindowCanvas(SDL_Window *, SDL_Renderer *);

    virtual int getHeight() override;
    virtual void setHeight(int height) override;

    virtual int getWidth() override;
    virtual void setWidth(int width) override;

    virtual CanvasRenderingContext2D * getContext() override;

  private:
    CanvasRenderingContext2D * context;
    SDL_Window * window;
};
