#pragma once

#include "../../../Canvas/CanvasRenderingContext2D.h"
#include "../../duktape.h"

namespace dcanvas
{
void initCanvas(duk_context * ctx, CanvasRenderingContext2D * canvas);
}
