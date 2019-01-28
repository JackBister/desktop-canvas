#pragma once

#include "../../duktape.h"

class CanvasRenderingContext2D;

namespace dcanvas {
	void initBitmap(duk_context * ctx, CanvasRenderingContext2D * canvas);
}
