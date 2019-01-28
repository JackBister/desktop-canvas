#include "DukJavaScriptEngine.h"

#include "duktape/bindings/bitmap/dukBitmap.h"
#include "duktape/bindings/canvas/dukCanvas.h"
#include "duktape/bindings/console/dukConsole.h"
#include "duktape/bindings/websocket/dukWebsocket.h"

#include "slurpFile.h"

static void pushJsValue(duk_context * ctx, JSValue value) {
	auto paramType = value.index();
	if (paramType == JSValue::Type::BOOL) {
		duk_push_boolean(ctx, std::get<bool>(value));
	} else if (paramType == JSValue::Type::DOUBLE) {
		duk_push_number(ctx, std::get<double>(value));
	} else if (paramType == JSValue::Type::OBJECT) {
		auto object = std::get<JSObject>(value);
		duk_push_bare_object(ctx);
		for (auto it = object.begin(); it != object.end(); ++it) {
			pushJsValue(ctx, it->second);
			duk_put_prop_string(ctx, -2, it->first.c_str());
		}
	} else if (paramType == JSValue::Type::STRING) {
		duk_push_string(ctx, std::get<std::string>(value).c_str());
	} else if (paramType == JSValue::Type::ARRAY) {
		auto arr = std::get<JSArray>(value);
		auto arrIdx = duk_push_array(ctx);
		for (size_t i = 0; i < arr.size(); ++i) {
			pushJsValue(ctx, arr[i]);
			duk_put_prop_index(ctx, arrIdx, i);
		}
	}
}

DukJavaScriptEngine::DukJavaScriptEngine() : ctx(duk_create_heap_default(), duk_destroy_heap)
{
	dcanvas::initConsole(ctx.get());
}

DukJavaScriptEngine::~DukJavaScriptEngine()
{
}

void DukJavaScriptEngine::callGlobalFunction(std::string const& function_name)
{
	duk_get_global_string(ctx.get(), function_name.c_str());
	duk_pcall(ctx.get(), 0);
	duk_pop(ctx.get());
}

void DukJavaScriptEngine::callGlobalFunction(std::string const& function_name, JSValue parameters)
{
	duk_get_global_string(ctx.get(), function_name.c_str());

	pushJsValue(ctx.get(), parameters);

	duk_pcall(ctx.get(), 1);
	duk_pop(ctx.get());
}

void DukJavaScriptEngine::evalFile(std::string const& filename)
{
	auto fileContents = dcanvas::slurpFile(filename);
	duk_eval_string(ctx.get(), fileContents.c_str());
}

void DukJavaScriptEngine::evalString(std::string const& str)
{
	duk_eval_string(ctx.get(), str.c_str());
}

void DukJavaScriptEngine::initBitmap(CanvasRenderingContext2D * canvas)
{
	dcanvas::initBitmap(ctx.get(), canvas);
}

void DukJavaScriptEngine::initCanvas(CanvasRenderingContext2D * canvas)
{
	dcanvas::initCanvas(ctx.get(), canvas);
}

void DukJavaScriptEngine::initWebsocket()
{
	dcanvas::initWebsocket(ctx.get(), &webSocketMessageQueue);
}

void DukJavaScriptEngine::preTick()
{
	for (size_t i = 0; i < webSocketMessageQueue.size(); ++i) {
		auto messageAndHandler = webSocketMessageQueue.pop();
		messageAndHandler.first(ctx.get(), messageAndHandler.second);
	}
}
