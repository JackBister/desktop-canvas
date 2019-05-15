#include "dukCanvas.h"

#include <string>

#include "../../../Canvas/Canvas.h"
#include "../../../Canvas/SDLOffscreenCanvas.h"
#include "../../../Logger/Logger.h"
#include "../../dukUtils.h"
#include "../../duktape.h"

#include "drawImage.h"

// TODO: Fix this
static SDL_Renderer * g_renderer = nullptr;

static void pushCanvasRenderingContext(duk_context * ctx, CanvasRenderingContext2D * canvas);

static void defineProp(duk_context * ctx, std::string const & name, int (*getter)(duk_context *))
{
    duk_push_string(ctx, name.c_str());
    duk_push_c_function(ctx, getter, 0);
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
}

static void defineProp(duk_context * ctx, std::string const & name, int (*getter)(duk_context *),
                       int (*setter)(duk_context *))
{
    duk_push_string(ctx, name.c_str());
    duk_push_c_function(ctx, getter, 0);
    duk_push_c_function(ctx, setter, 1);
    duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
}

static void defineMethod(duk_context * ctx, std::string const & name, int (*method)(duk_context *),
                         int nargs)
{
    duk_push_string(ctx, name.c_str());
    duk_push_c_function(ctx, method, nargs);
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);
}

static int getHeight(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<Canvas>(ctx);
    auto height = state->getHeight();
    duk_push_number(ctx, height);
    return 1;
}

static int getWidth(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<Canvas>(ctx);
    auto width = state->getWidth();
    duk_push_number(ctx, width);
    return 1;
}

static int getContext(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<Canvas>(ctx);
    pushCanvasRenderingContext(ctx, state->getContext());
    return 1;
}

static void pushCanvasToCtx(duk_context * ctx, Canvas * canvas)
{
    duk_push_object(ctx);
    duk_push_string(ctx, "\xFF"
                         "\xFF"
                         "internalClassName");
    duk_push_string(ctx, "Canvas");
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);
    duk_push_string(ctx, "\xFF"
                         "\xFF"
                         "internalPtr");
    duk_push_pointer(ctx, canvas);
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

    defineProp(ctx, "height", getHeight);
    defineProp(ctx, "width", getWidth);
    defineMethod(ctx, "getContext", getContext, 1);
}

static int getCanvas(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);
    auto canvas = state->getCanvas();

    pushCanvasToCtx(ctx, canvas);

    return 1;
}

static int fillRect(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    auto x = duk_require_int(ctx, -4);
    auto y = duk_require_int(ctx, -3);
    auto width = duk_require_int(ctx, -2);
    auto height = duk_require_int(ctx, -1);

    state->fillRect(x, y, width, height);

    return 0;
}

static int fillText(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    auto text = duk_require_string(ctx, -3);
    auto x = duk_require_int(ctx, -2);
    auto y = duk_require_int(ctx, -1);

    state->fillText(text, x, y);

    return 0;
}

static int getFillStyle(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    duk_push_string(ctx, state->getFillStyle().c_str());
    return 1;
}

static int setFillStyle(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    state->setFillStyle(std::string(duk_require_string(ctx, -1)));
    return 1;
}

static int getFont(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    duk_push_string(ctx, state->getFillStyle().c_str());
    return 1;
}

static int setFont(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    state->setFont(std::string(duk_require_string(ctx, -1)));
    return 1;
}

static void pushCanvasRenderingContext(duk_context * ctx, CanvasRenderingContext2D * canvas)
{
    duk_push_object(ctx);
    duk_push_string(ctx, "\xFF"
                         "\xFF"
                         "internalClassName");
    duk_push_string(ctx, "CanvasRenderingContext2D");
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);
    duk_push_string(ctx, "\xFF"
                         "\xFF"
                         "internalPtr");
    duk_push_pointer(ctx, canvas);
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

    defineProp(ctx, "fillStyle", getFillStyle, setFillStyle);
    defineProp(ctx, "font", getFont, setFont);

    defineMethod(ctx, "fillRect", fillRect, 4);
    defineMethod(ctx, "fillText", fillText, 3);
    defineMethod(ctx, "drawImage", dcanvas::drawImage, DUK_VARARGS);

    defineProp(ctx, "canvas", getCanvas);
}

static int offscreenCanvasConstructor(duk_context * ctx)
{
    auto w = duk_require_number(ctx, -2);
    auto h = duk_require_number(ctx, -1);
    // TODO: factory function
    auto ret = new SDLOffscreenCanvas(g_renderer, w, h);

    duk_push_object(ctx);
    duk_push_string(ctx, "\xFF"
                         "\xFF"
                         "internalClassName");
    duk_push_string(ctx, "Canvas");
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);
    duk_push_pointer(ctx, ret);
    duk_put_prop_string(ctx, -2,
                        "\xFF"
                        "\xFF"
                        "internalPtr");

    defineProp(ctx, "width", getWidth);
    defineProp(ctx, "height", getHeight);
    defineMethod(ctx, "getContext", getContext, 1);

    return 1;
}

namespace dcanvas
{
auto logger = Logger::get();

void initCanvas(duk_context * ctx, CanvasRenderingContext2D * canvas, SDL_Renderer * renderer)
{
    g_renderer = renderer;

    duk_push_c_function(ctx, offscreenCanvasConstructor, 2);
    duk_push_object(ctx);
    duk_put_prop_string(ctx, -2, "prototype");
    duk_put_global_string(ctx, "OffscreenCanvas");

    duk_get_global_string(ctx, "init");

    pushCanvasRenderingContext(ctx, canvas);

    duk_pcall(ctx, 1);
    logger->info("init call finished");
}
}
