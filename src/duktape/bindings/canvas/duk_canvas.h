#pragma once

#include "../../duktape.h"
#include "../../../Canvas/CanvasRenderingContext2D.h"

namespace dcanvas {
	void init_canvas(duk_context * ctx, ICanvasRenderingContext2D * canvas);
}
