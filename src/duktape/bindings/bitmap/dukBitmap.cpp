#include "dukBitmap.h"

#include <string>

#include "../../../Canvas/CanvasRenderingContext2D.h"
#include "../../../Logger/Logger.h"
#include "../../../slurpFile.h"

static auto logger = ILogger::get();

static ICanvasRenderingContext2D * canvas = nullptr;

static int finalizeBitmap(duk_context * ctx) {
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");
	auto bitmap = (Bitmap *)duk_get_pointer(ctx, -1);

	bitmap->close();

	duk_pop(ctx);
	return 0;
}

static int closeBitmap(duk_context * ctx) {
	duk_push_this(ctx);
	auto ret = finalizeBitmap(ctx);
	duk_pop(ctx);
	return ret;
}

static int loadBitmap(duk_context * ctx) {
	logger->info("loadBitmap");
	std::string filename(duk_require_string(ctx, -2));
	duk_require_function(ctx, -1);

	size_t fileLength = 0;
	auto file = dcanvas::slurpFile(filename.c_str(), &fileLength);

	auto bitmap = canvas->createBitmap(file, fileLength);

	duk_push_object(ctx);

	duk_push_pointer(ctx, bitmap);
	duk_put_prop_string(ctx, -2, "\xFF" "\xFF" "internalPtr");

	duk_push_string(ctx, "height");
	duk_push_number(ctx, bitmap->getHeight());
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);

	duk_push_string(ctx, "width");
	duk_push_number(ctx, bitmap->getWidth());
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE | DUK_DEFPROP_HAVE_WRITABLE | DUK_DEFPROP_HAVE_ENUMERABLE | DUK_DEFPROP_ENUMERABLE);

	duk_push_c_function(ctx, closeBitmap, 0);
	duk_put_prop_string(ctx, -2, "close");

	duk_push_c_function(ctx, finalizeBitmap, 1);
	duk_set_finalizer(ctx, -2);

	// Call the callback the user passed in with our newly constructed object as the argument
	duk_pcall(ctx, 1);

	duk_pop(ctx);

	return 0;
}

namespace dcanvas {
	void initBitmap(duk_context * ctx, ICanvasRenderingContext2D * cvs)
	{
		canvas = cvs;

		duk_push_global_object(ctx);
		duk_push_c_function(ctx, loadBitmap, 2);
		duk_put_prop_string(ctx, -2, "loadBitmap");
		duk_pop(ctx);
	}
}
