#include "SDLCanvasRenderingContext2D.h"

#include "../Logger/Logger.h"
#include "../hexToByte.h"

#include "./SDLBitmap.h"

static auto logger = Logger::get();

SDLCanvasRenderingContext2D::SDLCanvasRenderingContext2D(Canvas * canvas, SDL_Renderer * renderer,
                                                         SDL_Texture * renderTarget)
    : canvas(canvas), renderer(renderer), renderTarget(renderTarget), sdlFont(nullptr)
{
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
}

SDLBitmap * SDLCanvasRenderingContext2D::createBitmap(void * data, size_t size)
{
    auto rw = SDL_RWFromMem(data, size);
    auto surface = SDL_LoadBMP_RW(rw, 0);
    if (!surface) {
        logger->info("bmp surface NULL!");
    }

    auto texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        logger->info("texture NULL! %s", SDL_GetError());
    }

    return new SDLBitmap(texture, surface->h, surface->w);
}

void SDLCanvasRenderingContext2D::drawImage(Bitmap * img, int dx, int dy)
{
    setRenderTargetIfNeeded();
    auto image = ((SDLBitmap *)img)->getTexture();

    SDL_Rect dstRect;
    dstRect.x = dx;
    dstRect.y = dy;
    dstRect.w = img->getWidth();
    dstRect.h = img->getHeight();

    SDL_RenderCopy(renderer, image, nullptr, &dstRect);
}

void SDLCanvasRenderingContext2D::drawImage(Bitmap * img, int dx, int dy, int dWidth, int dHeight)
{
    setRenderTargetIfNeeded();
    auto image = ((SDLBitmap *)img)->getTexture();

    SDL_Rect dstRect;
    dstRect.x = dx;
    dstRect.y = dy;
    dstRect.w = dWidth;
    dstRect.h = dHeight;
    SDL_RenderCopy(renderer, image, nullptr, &dstRect);
}

void SDLCanvasRenderingContext2D::drawImage(Bitmap * img, int sx, int sy, int sWidth, int sHeight,
                                            int dx, int dy, int dWidth, int dHeight)
{
    setRenderTargetIfNeeded();
    auto image = ((SDLBitmap *)img)->getTexture();

    SDL_Rect srcRect;
    srcRect.x = sx;
    srcRect.y = sy;
    srcRect.w = sWidth;
    srcRect.h = sHeight;

    SDL_Rect dstRect;
    dstRect.x = dx;
    dstRect.y = dy;
    dstRect.w = dWidth;
    dstRect.h = dHeight;
    SDL_RenderCopy(renderer, image, &srcRect, &dstRect);
}

void SDLCanvasRenderingContext2D::fillRect(int x, int y, int width, int height)
{
    setRenderTargetIfNeeded();
    auto r = dcanvas::hexToByte(fillStyle.substr(1, 2));
    auto g = dcanvas::hexToByte(fillStyle.substr(3, 2));
    auto b = dcanvas::hexToByte(fillStyle.substr(5, 2));

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

void SDLCanvasRenderingContext2D::fillText(std::string const & text, int x, int y)
{
    setRenderTargetIfNeeded();
    auto r = dcanvas::hexToByte(fillStyle.substr(1, 2));
    auto g = dcanvas::hexToByte(fillStyle.substr(3, 2));
    auto b = dcanvas::hexToByte(fillStyle.substr(5, 2));

    SDL_Color fg;
    fg.r = r;
    fg.g = g;
    fg.b = b;
    fg.a = 0xFF;

    auto surf = TTF_RenderUTF8_Solid(sdlFont.get(), text.c_str(), fg);
    if (!surf) {
        logger->info("Unable to render text: %s", SDL_GetError());
    }
    auto tex = SDL_CreateTextureFromSurface(renderer, surf);

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.h = surf->h;
    dst.w = surf->w;

    if (SDL_RenderCopy(renderer, tex, nullptr, &dst)) {
        logger->info("RenderCopy failed: %s", SDL_GetError());
    }

    SDL_DestroyTexture(tex);
    // SDL_FreeSurface(surf);
}

std::string const & SDLCanvasRenderingContext2D::getFont()
{
    return font;
}

void SDLCanvasRenderingContext2D::setFont(std::string const & val)
{
    font = val;

    auto sizeAndFont = parseFont(val);
    sdlFont.reset(TTF_OpenFont(sizeAndFont.second.c_str(), sizeAndFont.first));
}

std::pair<int, std::string> SDLCanvasRenderingContext2D::parseFont(std::string const & fontString)
{
    auto pIndex = fontString.find('p');
    auto sizeString = fontString.substr(0, pIndex);
    auto size = std::stoi(sizeString);

    auto spaceIndex = fontString.find(' ');
    auto fontPath = fontString.substr(spaceIndex + 1);

    return {size, fontPath};
}

void SDLCanvasRenderingContext2D::setRenderTargetIfNeeded()
{
    auto currentTarget = SDL_GetRenderTarget(renderer);
    if (currentTarget != renderTarget) {
        SDL_SetRenderTarget(renderer, renderTarget);
        // TODO: SetLogicalSize(?)
    }
}
