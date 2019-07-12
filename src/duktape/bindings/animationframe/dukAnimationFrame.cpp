#include "dukAnimationFrame.h"

#include "../../dukUtils.h"
#include "../../../Logger/Logger.h"

static auto logger = Logger::get();

static int requestAnimationFrame(duk_context * ctx)
{
    duk_require_function(ctx, -1);

    duk_push_global_stash(ctx); //fn, globalStash
    duk_get_prop_string(ctx, -1, "__nextAnimationFrameFunctions"); // fn, globalStash, (array or undefined)

    if (duk_is_undefined(ctx, -1)) {
        duk_pop(ctx); // fn, globalStash
        duk_push_array(ctx); // fn, globalStash, array
        duk_put_prop_string(ctx, -2, "__nextAnimationFrameFunctions"); // fn, globalStash
        duk_get_prop_string(ctx, -1, "__nextAnimationFrameFunctions"); // fn, globalStash, array
    }

    auto numHandlers = duk_get_length(ctx, -1);
    duk_swap_top(ctx, -3); // array, globalStash, fn
    duk_require_function(ctx, -1);
    duk_put_prop_index(ctx, -3,
                       numHandlers); // array, globalStash
    duk_pop_2(ctx);
    return 0;
}

static int cancelAnimationFrame(duk_context* ctx)
{
    duk_require_function(ctx, -1);

    duk_push_global_stash(ctx);                      // fn, globalStash
    duk_get_prop_string(ctx, -1, "__nextAnimationFrameFunctions"); // fn, globalStash, (array or undefined)
  
    if (duk_is_undefined(ctx, -1)) {
        duk_pop_3(ctx);
        return 0;
    }
    // fn, globalStash, array of handlers
    auto numHandlers = duk_get_length(ctx, -1);
    for (auto i = 0; i < numHandlers; ++i) {
        duk_get_prop_index(
            ctx, -1, i); // fn, globalStash, array of handlers, handler
        duk_require_function(ctx, -1);
        duk_require_function(ctx, -4);
        if (duk_equals(ctx, -1, -4)) {
            // TODO: breaks if there are multiple hits
            dcanvas::dukUtils::arraySplice(ctx, -2, i, 1);
        }
        duk_pop(ctx);
    }
    duk_pop_3(ctx);
    return 0;
}

void dcanvas::executeAnimationFrameFunctions(duk_context * ctx)
{
    duk_push_global_stash(ctx); // globalStash
    duk_get_prop_string(
        ctx, -1,
        "__nextAnimationFrameFunctions"); // globalStash, (undefined or array of handlers)
    if (duk_is_undefined(ctx, -1)) {
        duk_pop_2(ctx);
        return;
    }
    // globalStash, array of handlers
    duk_put_prop_string(ctx, -2, "__animationFrameFunctions"); // globalStash
    duk_push_array(ctx); // globalStash, []
    duk_put_prop_string(ctx, -2, "__nextAnimationFrameFunctions"); // globalStash
    duk_get_prop_string(ctx, -1, "__animationFrameFunctions"); // globalStash, array of handlers
    auto numHandlers = duk_get_length(ctx, -1);
    for (auto i = 0; i < numHandlers; ++i) {
        duk_get_prop_index(ctx, -1, i); // globalStash, array of handlers, handler
        auto result =
            duk_pcall(ctx, 0); // globalStash, __eventHandlers, array of handlers, (ret or err)
        if (result != 0) {
            duk_safe_to_string(ctx, -1);
            logger->info("Got error when running animation frame function, error=%s", duk_get_string(ctx, -1));
            // TODO: Actually do something
        }
        // TODO: Does this work with void functions?
        duk_pop(ctx); // globalStash, array of handlers
    }
    duk_pop(ctx); // globalStash
    duk_push_array(ctx); // globalStash, []
    duk_put_prop_string(ctx, -2, "__animationFrameFunctions"); // globalStash
    duk_pop(ctx);
}

void dcanvas::initAnimationFrame(duk_context * ctx)
{
    duk_push_global_object(ctx);

    duk_push_c_function(ctx, requestAnimationFrame, 1);
    duk_put_prop_string(ctx, -2, "requestAnimationFrame");

    duk_push_c_function(ctx, cancelAnimationFrame, 1);
    duk_put_prop_string(ctx, -2, "cancelAnimationFrame");

    duk_pop(ctx);
}
