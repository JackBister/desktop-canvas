#include "drawImage.h"

#include "../../dukUtils.h"
#include "../../duktape.h"

#include "../../../Canvas/CanvasRenderingContext2D.h"
#include "../../../Canvas/SDLOffscreenCanvas.h"

static int drawImage3args(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    duk_require_object(ctx, -3);
    int dx = duk_require_number(ctx, -2);
    int dy = duk_require_number(ctx, -1);

    duk_get_prop_string(ctx, -3,
                        "\xFF"
                        "\xFF"
                        "internalClassName");
    std::string className(duk_require_string(ctx, -1));
    duk_pop(ctx);
    duk_get_prop_string(ctx, -3,
                        "\xFF"
                        "\xFF"
                        "internalPtr");
    if (className == "Canvas") {
        auto canvas = (SDLOffscreenCanvas *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        auto bitmap = canvas->asBitmap();
        state->drawImage(bitmap.get(), dx, dy);
    } else {
        auto bitmap = (SDLBitmap *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        state->drawImage(bitmap, dx, dy);
    }

    return 0;
}

static int drawImage5args(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    duk_require_object(ctx, -5);
    int dx = duk_require_number(ctx, -4);
    int dy = duk_require_number(ctx, -3);
    int dWidth = duk_require_number(ctx, -2);
    int dHeight = duk_require_number(ctx, -1);

    duk_get_prop_string(ctx, -5,
                        "\xFF"
                        "\xFF"
                        "internalClassName");
    std::string className(duk_require_string(ctx, -1));
    duk_pop(ctx);
    duk_get_prop_string(ctx, -5,
                        "\xFF"
                        "\xFF"
                        "internalPtr");
    if (className == "Canvas") {
        auto canvas = (SDLOffscreenCanvas *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        auto bitmap = canvas->asBitmap();
        state->drawImage(bitmap.get(), dx, dy, dWidth, dHeight);
    } else {
        auto bitmap = (SDLBitmap *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        state->drawImage(bitmap, dx, dy, dWidth, dHeight);
    }

    return 0;
}

static int drawImage9args(duk_context * ctx)
{
    auto state = dcanvas::dukUtils::fromCtx<CanvasRenderingContext2D>(ctx);

    duk_require_object(ctx, -9);
    int sx = duk_require_number(ctx, -8);
    int sy = duk_require_number(ctx, -7);
    int sWidth = duk_require_number(ctx, -6);
    int sHeight = duk_require_number(ctx, -5);
    int dx = duk_require_number(ctx, -4);
    int dy = duk_require_number(ctx, -3);
    int dWidth = duk_require_number(ctx, -2);
    int dHeight = duk_require_number(ctx, -1);

    duk_get_prop_string(ctx, -9,
                        "\xFF"
                        "\xFF"
                        "internalClassName");
    std::string className(duk_require_string(ctx, -1));
    duk_pop(ctx);
    duk_get_prop_string(ctx, -9,
                        "\xFF"
                        "\xFF"
                        "internalPtr");
    if (className == "Canvas") {
        auto canvas = (SDLOffscreenCanvas *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        auto bitmap = canvas->asBitmap();
        state->drawImage(bitmap.get(), sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight);
    } else {
        auto bitmap = (SDLBitmap *)duk_get_pointer(ctx, -1);
        duk_pop(ctx);
        state->drawImage(bitmap, sx, sy, sWidth, sHeight, dx, dy, dWidth, dHeight);
    }

    return 0;
}

int dcanvas::drawImage(duk_context * ctx)
{
    switch (duk_get_top(ctx)) {
    case 3:
        return drawImage3args(ctx);
    case 5:
        return drawImage5args(ctx);
    case 9:
        return drawImage9args(ctx);
    }

    duk_throw(ctx);
    return 0;
}
