#pragma once

class CanvasRenderingContext2D;

#include "../../duktape.h"

namespace dcanvas
{
CanvasRenderingContext2D * canvasFromCtx(duk_context * ctx);
}
