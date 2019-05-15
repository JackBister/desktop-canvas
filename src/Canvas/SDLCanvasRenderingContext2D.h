#pragma once
#include "CanvasRenderingContext2D.h"

#include <memory>

#ifdef __ANDROID__
#include <SDL2/SDL_config_android.h>
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "SDLBitmap.h"

class Canvas;

class SDLCanvasRenderingContext2D : public CanvasRenderingContext2D
{
  public:
    SDLCanvasRenderingContext2D(Canvas * canvas, SDL_Renderer * renderer,
                                SDL_Texture * renderTarget);

    virtual SDLBitmap * createBitmap(void * data, size_t size) override;

    virtual void drawImage(Bitmap * image, int dx, int dy) override;
    virtual void drawImage(Bitmap * image, int dx, int dy, int dWidth, int dHeight) override;
    virtual void drawImage(Bitmap * image, int sx, int sy, int sWidth, int sHeight, int dx, int dy,
                           int dWidth, int dHeight) override;

    virtual void fillRect(int x, int y, int width, int height) override;

    virtual void fillText(std::string const & text, int x, int y) override;

    virtual std::string const & getFont() override;
    virtual void setFont(std::string const & val) override;

    virtual std::string const & getFillStyle() override { return fillStyle; }
    virtual void setFillStyle(std::string const & val) override { fillStyle = val; }

    virtual Canvas * getCanvas() override { return canvas; }

  private:
    struct FontDestroyer {
        void operator()(TTF_Font * font) { TTF_CloseFont(font); }
    };

    Canvas * canvas;
    SDL_Renderer * renderer;
    SDL_Texture * renderTarget;

    std::unique_ptr<TTF_Font, FontDestroyer> sdlFont;

    std::string fillStyle = "#000000";
    std::string font = "";
    std::string strokeStyle = "#000000";

    std::pair<int, std::string> parseFont(std::string const &);
    void setRenderTargetIfNeeded();
};
