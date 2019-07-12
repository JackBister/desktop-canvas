#include "SDLCanvasRenderingContext2D.h"

#include "../Logger/Logger.h"
#include "../hexToByte.h"

#include "./SDLBitmap.h"

static auto logger = Logger::get();

struct Color
{
    uint8_t r, g, b;
};

static Color parseColor(std::string const& color) {
    Color ret;
    if (color.size() == 7) {
        ret.r = dcanvas::hexToByte(color.substr(1, 2));
        ret.g = dcanvas::hexToByte(color.substr(3, 2));
        ret.b = dcanvas::hexToByte(color.substr(5, 2));
    } else if (color.size() == 4) {
        auto r = dcanvas::hexCharToByte(color[1]);
        auto g = dcanvas::hexCharToByte(color[2]);
        auto b = dcanvas::hexCharToByte(color[3]);
        ret.r = r | r << 4;
        ret.g = g | g << 4;
        ret.b = b | b << 4;
    } else {
        logger->info("parseColor: Unable to parse color string '%s', defaulting to #D500FF", color.c_str());
        ret.r = 0xD5;
        ret.g = 0;
        ret.b = 0xFF;
    }
    return ret;
}

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
    Color fillColor = parseColor(fillStyle);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;

    SDL_SetRenderDrawColor(renderer, fillColor.r, fillColor.g, fillColor.b, 0xFF);
    SDL_RenderFillRect(renderer, &rect);
}

void SDLCanvasRenderingContext2D::fillText(std::string const & text, int x, int y)
{
    setRenderTargetIfNeeded();
    Color fillColor = parseColor(fillStyle);

    SDL_Color fg;
    fg.r = fillColor.r;
    fg.g = fillColor.g;
    fg.b = fillColor.b;
    fg.a = 0xFF;

	int width, height;
	TTF_SizeUTF8(sdlFont.get(), text.c_str(), &width, &height);
	if (textAlign == TextAlign::CENTER) {
        x -= width / 2;
	}

	if (textBaseline == TextBaseline::MIDDLE) {
		y -= height / 2;
	}

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
	SDL_FreeSurface(surf);
}

TextMetrics SDLCanvasRenderingContext2D::measureText(std::string const & text)
{
    int width, height;
    TTF_SizeText(sdlFont.get(), text.c_str(), &width, &height);
    return TextMetrics(width);
}

std::string const & SDLCanvasRenderingContext2D::getTextAlign()
{
    switch (textAlign) {
    case TextAlign::CENTER:
        return "center";
    case TextAlign::LEFT:
        return "left";
	default:
		logger->info("Unknown textAlign %d when getting, defaulting to left", textAlign);
		return "left";
	}
}

void SDLCanvasRenderingContext2D::setTextAlign(std::string const & val)
{
    if (val == "center") {
        textAlign = TextAlign::CENTER;
    } else if (val == "left") {
        textAlign = TextAlign::LEFT;
	} else {
		logger->info("Unknown textAlign '%s' when setting, defaulting to left", val.c_str());
        textAlign = TextAlign::LEFT;
	}
}

std::string const & SDLCanvasRenderingContext2D::getTextBaseline()
{
	switch (textBaseline) {
	case TextBaseline::MIDDLE:
		return "middle";
	case TextBaseline::TOP:
		return "top";
	default:
		logger->info("Unknown textBaseline %d when getting, defaulting to top", textBaseline);
		return "top";
	}
}

void SDLCanvasRenderingContext2D::setTextBaseline(std::string const & val)
{
	if (val == "middle") {
		textBaseline = TextBaseline::MIDDLE;
	} else if (val == "top") {
		textBaseline = TextBaseline::TOP;
	} else {
		logger->info("Unknown textBaseline '%s' when setting, defaulting to top", val.c_str());
		textBaseline = TextBaseline::TOP;
	}
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
    if (!sdlFont) {
        logger->info("Error loading font: %s", TTF_GetError());
    }
}

std::pair<int, std::string> SDLCanvasRenderingContext2D::parseFont(std::string const & fontString)
{
    auto pIndex = fontString.find('p');
    auto sizeString = fontString.substr(0, pIndex);
    auto size = std::stoi(sizeString);

    auto spaceIndex = fontString.find_last_of(' ');
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
