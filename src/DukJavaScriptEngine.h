#pragma once

#include "JavaScriptEngine.h"

#include <functional>

#include "duktape/duktape.h"
#include "WebSocket/MessageEvent.h"
#include "SafeQueue.h"

struct SDL_Renderer;
struct SDL_Surface;

class DukJavaScriptEngine : public IJavaScriptEngine {
public:
	DukJavaScriptEngine();
	~DukJavaScriptEngine() override;

	virtual void call_global_function(std::string const & function_name) override;
	virtual void call_global_function(std::string const& function_name, JSValue parameters) override;
	virtual void eval_file(std::string const & filename) override;
	virtual void eval_string(std::string const & str) override;
	virtual void init_bitmap(ICanvasRenderingContext2D *) override;
	virtual void init_canvas(ICanvasRenderingContext2D *) override;
	virtual void init_websocket() override;
	virtual void pre_tick() override;

private:
	using DukDestroyer = void(*)(duk_context *);
	std::unique_ptr<duk_context, DukDestroyer> ctx;

	SafeQueue<std::pair<std::function<void(duk_context *, MessageEvent)>, MessageEvent>> webSocketMessageQueue;
};
