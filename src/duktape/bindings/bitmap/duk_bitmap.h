#pragma once

#include "../../duktape.h"

class ICanvasRenderingContext2D;

namespace dcanvas {
	void init_bitmap(duk_context * ctx, ICanvasRenderingContext2D * canvas);
}
