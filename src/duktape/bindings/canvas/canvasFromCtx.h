#pragma once

class ICanvasRenderingContext2D;

#include "../../duktape.h"

namespace dcanvas {
	ICanvasRenderingContext2D * canvasFromCtx(duk_context * ctx);
}
