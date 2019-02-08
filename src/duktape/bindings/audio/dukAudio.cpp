#include "dukAudio.h"

#include <string>

#include "../../../Audio/SDLAudio.h"
#include "../../../Logger/Logger.h"
#include "../../dukUtils.h"
#include "../../duktape.h"

static auto logger = Logger::get();

static int pause(duk_context * ctx)
{
    auto audio = dcanvas::dukUtils::fromCtx<Audio>(ctx);
    audio->pause();
    return 0;
}

static int play(duk_context * ctx)
{
    auto audio = dcanvas::dukUtils::fromCtx<Audio>(ctx);
    audio->play();
    return 0;
}

static int audioConstructor(duk_context * ctx)
{
    std::string src(duk_require_string(ctx, -1));

    auto ret = new SDLAudio(src);

    duk_push_object(ctx);

    duk_push_pointer(ctx, ret);
    duk_put_prop_string(ctx, -2,
                        "\xFF"
                        "\xFF"
                        "internalPtr");

    duk_push_string(ctx, "pause");
    duk_push_c_function(ctx, pause, 0);
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

    duk_push_string(ctx, "play");
    duk_push_c_function(ctx, play, 0);
    duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

    return 1;
}

void dcanvas::initAudio(duk_context * ctx)
{
    duk_push_c_function(ctx, audioConstructor, 1);
    duk_push_object(ctx);
    duk_put_prop_string(ctx, -2, "prototype");
    duk_put_global_string(ctx, "Audio");
    // duk_pop(ctx);
    logger->info("initAudio end dukTop %d", duk_get_top(ctx));
}
