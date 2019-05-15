#pragma once

#include "../../../Canvas/CanvasRenderingContext2D.h"
#include "../../duktape.h"

struct SDL_Renderer;

namespace dcanvas
{
void initCanvas(duk_context * ctx, CanvasRenderingContext2D * canvas, SDL_Renderer * renderer);
}
