#include "dukWebsocket.h"

#include <string>
#include <sstream>

#include "../../../Logger/Logger.h"
#include "../../../WebSocket/WebSocketpp.h"

static auto logger = ILogger::get();
static SafeQueue<std::pair<std::function<void(duk_context *, MessageEvent)>, MessageEvent>> * webSocketMessageQueue = nullptr;

static int close(duk_context * ctx) {
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");

	auto ws = (WebSocket *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx);

	ws->close();

	return 0;
}

static int getReadyState(duk_context * ctx) {
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");

	auto ws = (WebSocket *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx);
	auto readyState = ws->getReadyState();
	duk_push_number(ctx, readyState);

	return 1;
}

static int setOnMessage(duk_context * ctx) {
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");

	auto ws = (WebSocket *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx);

	duk_require_callable(ctx, -1);
	duk_push_global_stash(ctx);
	duk_swap_top(ctx, -2);
	// TODO: Yikes
	std::stringstream ss;
	ss << (void *) ws;
	duk_put_prop_string(ctx, -2, ss.str().c_str());
	duk_pop(ctx);

	ws->setOnMessage([ws](MessageEvent msg) {
		webSocketMessageQueue->push(
		std::make_pair(
			[ws](duk_context * ctx, MessageEvent e) {
				duk_push_global_stash(ctx);
				// TODO: Double yikes
				std::stringstream ss;
				ss << (void *)ws;
				duk_get_prop_string(ctx, -1, ss.str().c_str());
				duk_push_object(ctx);
				duk_push_string(ctx, e.data.c_str());
				duk_put_prop_string(ctx, -2, "data");
				duk_call(ctx, 1);
				duk_pop_2(ctx);
			}
			, msg
		));
	});

	return 0;
}

static int wsSend(duk_context * ctx) {
	duk_push_this(ctx);
	duk_get_prop_string(ctx, -1, "\xFF" "\xFF" "internalPtr");

	auto ws = (WebSocket *)duk_get_pointer(ctx, -1);
	duk_pop_2(ctx);

	std::string data(duk_require_string(ctx, -1));

	ws->send(data);

	return 0;
}

static int websocketConstructor(duk_context * ctx) {
	logger->info("WebSocket constructor");

	std::string url(duk_require_string(ctx, -1));

	auto ret = new WebSocketpp(url);

	duk_push_object(ctx);

	duk_push_pointer(ctx, ret);
	duk_put_prop_string(ctx, -2, "\xFF" "\xFF" "internalPtr");

	duk_push_string(ctx, "readyState");
	duk_push_c_function(ctx, getReadyState, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

	duk_push_string(ctx, "onmessage");
	duk_push_c_function(ctx, setOnMessage, 1);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_SETTER);

	duk_push_string(ctx, "close");
	duk_push_c_function(ctx, close, 0);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

	duk_push_string(ctx, "send");
	duk_push_c_function(ctx, wsSend, 1);
	duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

	return 1;
}

void dcanvas::initWebsocket(duk_context * ctx, SafeQueue<std::pair<std::function<void(duk_context *, MessageEvent)>, MessageEvent>> * messageQueue)
{
	webSocketMessageQueue = messageQueue;

	duk_push_c_function(ctx, websocketConstructor, 1);
	duk_push_object(ctx);
	duk_put_prop_string(ctx, -2, "prototype");
	duk_put_global_string(ctx, "WebSocket");
	duk_pop(ctx);
}
