#pragma once

#include "../../duktape.h"
#include "../../../WebSocket/MessageEvent.h"
#include "../../../SafeQueue.h"

namespace dcanvas {
	// TODO: Should probably take a WebsocketFactory parameter
	void init_websocket(duk_context * ctx, SafeQueue<std::pair<std::function<void(duk_context *, MessageEvent)>, MessageEvent>> * webSocketMessageQueue);
}
