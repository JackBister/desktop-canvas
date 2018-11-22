#pragma once

class ICanvasRenderingContext2D;

#include "../../duktape.h"

namespace dcanvas {
	ICanvasRenderingContext2D * canvas_from_ctx(duk_context * ctx);
}
