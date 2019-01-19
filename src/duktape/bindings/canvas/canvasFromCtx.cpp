#include "canvasFromCtx.h"

ICanvasRenderingContext2D * dcanvas::canvasFromCtx(duk_context * ctx)
{
	duk_push_this(ctx);
	duk_require_object(ctx, -1);
	duk_push_string(ctx, "\xFF" "\xFF" "internalPtr");
	duk_get_prop(ctx, -2);
	auto state = (ICanvasRenderingContext2D *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx);

	return state;
}
