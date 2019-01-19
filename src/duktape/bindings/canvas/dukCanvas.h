#pragma once

#include "../../duktape.h"
#include "../../../Canvas/CanvasRenderingContext2D.h"

namespace dcanvas {
	void initCanvas(duk_context * ctx, ICanvasRenderingContext2D * canvas);
}
