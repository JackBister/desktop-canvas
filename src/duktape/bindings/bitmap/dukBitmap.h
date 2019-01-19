#pragma once

#include "../../duktape.h"

class ICanvasRenderingContext2D;

namespace dcanvas {
	void initBitmap(duk_context * ctx, ICanvasRenderingContext2D * canvas);
}
