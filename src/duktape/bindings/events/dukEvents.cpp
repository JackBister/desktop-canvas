#include "dukEvents.h"

#include "../../dukUtils.h"
#include "../../../Logger/Logger.h"

static auto logger = Logger::get();

static int addEventListener(duk_context * ctx)
{
    std::string eventName(duk_require_string(ctx, -2));
    duk_require_function(ctx, -1);

	duk_push_global_stash(ctx); // eventName, fn, globalStash
    duk_get_prop_string(ctx, -1, "__eventHandlers"); // eventName, fn, globalStash, __eventHandlers
    duk_get_prop_string(ctx, -1, eventName.c_str()); // eventName, fn, globalStash, __eventHandlers, (undefined or array of handlers)
    if (duk_is_undefined(ctx, -1)) {
        duk_pop(ctx); // eventName, fn, globalStash, __eventHandlers
        duk_push_array(ctx); // eventName, fn, globalStash, __eventHandlers, array of handlers
		duk_put_prop_string(ctx, -2, eventName.c_str()); // eventName, fn, globalStash, __eventHandlers
		duk_get_prop_string(ctx, -1, eventName.c_str()); // eventName, fn, globalStash, __eventHandlers, array of handlers
	}
    auto numHandlers = duk_get_length(ctx, -1);
	duk_swap_top(ctx, -4); // eventName, array of handlers, globalStash, __eventHandlers, fn
	duk_require_function(ctx, -1);
    duk_put_prop_index(ctx, -4, numHandlers); // eventName, array of handlers, globalStash, __eventHandlers
    duk_pop_n(ctx, 4);
    return 0;
}

static int removeEventListener(duk_context * ctx)
{
    std::string eventName(duk_require_string(ctx, -2));
    duk_require_function(ctx, -1);

	duk_push_global_stash(ctx); // eventName, fn, globalStash
    duk_get_prop_string(ctx, -1, "__eventHandlers"); // eventName, fn, globalStash, __eventHandlers
    duk_get_prop_string(ctx, -1, eventName.c_str()); // eventName, fn, globalStash, __eventHandlers, (undefined or array of handlers)
	if (duk_is_undefined(ctx, -1)) {
		return 0;
	}
	// eventName, fn, globalStash, __eventHandlers, array of handlers
	auto numHandlers = duk_get_length(ctx, -1);
	for (auto i = 0; i < numHandlers; ++i) {
		duk_get_prop_index(ctx, -1, i); // eventName, fn, globalStash, __eventHandlers, array of handlers, handler
		duk_require_function(ctx, -1);
		duk_require_function(ctx, -5);
		if (duk_equals(ctx, -1, -5)) {
			// TODO: breaks if there are multiple hits
			dcanvas::dukUtils::arraySplice(ctx, -2, i, 1);
		}
		duk_pop(ctx);
	}
	return 0;
}

void dcanvas::initEvents(duk_context * ctx)
{
    duk_push_global_object(ctx); // globalObject
    duk_push_c_function(ctx, addEventListener, 2); // globalObject, addEventListener
    duk_put_prop_string(ctx, -2, "addEventListener"); // globalObject
	duk_push_c_function(ctx, removeEventListener, 2); // globalObject, removeEventListener
    duk_put_prop_string(ctx, -2, "removeEventListener"); // globalObject
    duk_pop(ctx);

	duk_push_global_stash(ctx); // globalStash
	duk_push_object(ctx); // globalStash, {}
	duk_put_prop_string(ctx, -2, "__eventHandlers"); // globalStash
	duk_pop(ctx);
}

void dcanvas::callEventListeners(duk_context * ctx, std::string const & eventName, JSValue evt)
{
    duk_push_global_stash(ctx); // globalStash
    duk_get_prop_string(ctx, -1, "__eventHandlers"); // globalStash, __eventHandlers
    duk_get_prop_string(ctx, -1, eventName.c_str()); // globalStash, __eventHandlers, (undefined or array of handlers)
    if (duk_is_undefined(ctx, -1)) {
        return;
	}
	// globalStash, __eventHandlers, array of handlers
	auto numHandlers = duk_get_length(ctx, -1);
	for (auto i = 0; i < numHandlers; ++i) {
		duk_get_prop_index(ctx, -1, i); // globalStash, __eventHandlers, array of handlers, handler
		dcanvas::dukUtils::pushToCtx(ctx, evt); // globalStash, __eventHandlers, array of handlers, handler, evt
		auto result = duk_pcall(ctx, 1); // globalStash, __eventHandlers, array of handlers, (ret or err)
		if (result != 0) {
			duk_safe_to_string(ctx, -1);
			logger->info("Got error when handling eventName=%s, error=%s", eventName.c_str(), duk_get_string(ctx, -1));
			// TODO: Actually do something
		}
		// TODO: Does this work with void functions?
		duk_pop(ctx); // globalStash, __eventHandlers, array of handlers
	}
	duk_pop_3(ctx);
}
