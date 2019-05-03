#include "DukJavaScriptEngine.h"

#include "duktape/bindings/audio/dukAudio.h"
#include "duktape/bindings/bitmap/dukBitmap.h"
#include "duktape/bindings/canvas/dukCanvas.h"
#include "duktape/bindings/console/dukConsole.h"
#include "duktape/bindings/events/dukEvents.h"
#include "duktape/bindings/navigator/dukNavigator.h"
#include "duktape/bindings/websocket/dukWebsocket.h"
#include "duktape/dukUtils.h"

#include "slurpFile.h"

DukJavaScriptEngine::DukJavaScriptEngine() : ctx(duk_create_heap_default(), duk_destroy_heap)
{
    dcanvas::initConsole(ctx.get());
    dcanvas::initEvents(ctx.get());
}

DukJavaScriptEngine::~DukJavaScriptEngine() {}

void DukJavaScriptEngine::callEventListeners(std::string const & eventName, JSValue evt)
{
    dcanvas::callEventListeners(ctx.get(), eventName, evt);
}

void DukJavaScriptEngine::callGlobalFunction(std::string const & function_name)
{
    duk_get_global_string(ctx.get(), function_name.c_str());
    duk_pcall(ctx.get(), 0);
    duk_pop(ctx.get());
}

void DukJavaScriptEngine::callGlobalFunction(std::string const & function_name, JSValue parameters)
{
    printf("p %d\n", duk_get_top(ctx.get()));
    duk_get_global_string(ctx.get(), function_name.c_str());
    printf("f %d\n", duk_get_top(ctx.get()));
    dcanvas::dukUtils::pushToCtx(ctx.get(), parameters);
    printf("%d\n", duk_get_top(ctx.get()));
    duk_call(ctx.get(), 1);
    //duk_pcall(ctx.get(), 1);
    duk_pop(ctx.get());
}

JSValue DukJavaScriptEngine::callGlobalFunctionWithReturn(std::string const & functionName)
{
    duk_get_global_string(ctx.get(), functionName.c_str());
    duk_pcall(ctx.get(), 0);
    auto ret = dcanvas::dukUtils::pullFromCtx(ctx.get());
    duk_pop(ctx.get());
	return ret;
}

void DukJavaScriptEngine::evalFile(std::string const & filename)
{
    auto fileContents = dcanvas::slurpFile(filename);
    duk_eval_string(ctx.get(), fileContents.c_str());
}

void DukJavaScriptEngine::evalString(std::string const & str)
{
    duk_eval_string(ctx.get(), str.c_str());
}

void DukJavaScriptEngine::initAudio()
{
    dcanvas::initAudio(ctx.get());
}

void DukJavaScriptEngine::initBitmap(CanvasRenderingContext2D * canvas)
{
    dcanvas::initBitmap(ctx.get(), canvas);
}

void DukJavaScriptEngine::initCanvas(CanvasRenderingContext2D * canvas)
{
    dcanvas::initCanvas(ctx.get(), canvas);
}

void DukJavaScriptEngine::initNavigator(Navigator * navigator)
{
    dcanvas::initNavigator(ctx.get(), navigator);
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
