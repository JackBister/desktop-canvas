#include "CanvasState.h"

#include "../../duktape.h"

static uint8_t hexCharToByte(char hex) {
	if (hex > 'A' && hex <= 'F') {
		return 10 + hex - 'A';
	}
	if (hex > 'a' && hex <= 'f') {
		return 10 + hex - 'a';
	}
	return hex - '0';
}

static uint8_t hexToByte(std::string_view hexString) {
	return hexCharToByte(hexString[0]) << 4 | hexCharToByte(hexString[1]);
}

Color CanvasState::getFillColor()
{
	auto r = fillStyle.substr(1, 2);
	auto g = fillStyle.substr(3, 2);
	auto b = fillStyle.substr(5, 2);

	return Color{
		hexToByte(r),
		hexToByte(g),
		hexToByte(b)
	};
}

CanvasState * CanvasState::fromThis(duk_context * ctx)
{
	duk_push_this(ctx);
	duk_require_object(ctx, -1);
	duk_push_string(ctx, "\xFF" "\xFF" "internalPtr");
	duk_get_prop(ctx, -2);
	auto state = (CanvasState *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx);

	return state;
}

void CanvasState::drawImage(SDL_Texture * image, int dx, int dy)
{
	int height = 0;
	int width = 0;
	// TODO: This sucks
	SDL_QueryTexture(image, nullptr, nullptr, &width, &height);


	SDL_Rect dstRect;
	dstRect.x = dx;
	dstRect.y = dy;
	dstRect.w = width;
	dstRect.h = height;
	SDL_RenderCopy(renderer, image, nullptr, &dstRect);
}

void CanvasState::drawImage(SDL_Texture * image, int dx, int dy, int dWidth, int dHeight)
{
	SDL_Rect dstRect;
	dstRect.x = dx;
	dstRect.y = dy;
	dstRect.w = dWidth;
	dstRect.h = dHeight;
	SDL_RenderCopy(renderer, image, nullptr, &dstRect);
}

void CanvasState::drawImage(SDL_Texture * image, int sx, int sy, int sWidth, int sHeight, int dx, int dy, int dWidth, int dHeight)
{
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
