#pragma once

#include "JavaScriptEngine.h"

#include <functional>

#include "SafeQueue.h"
#include "WebSocket/MessageEvent.h"
#include "duktape/duktape.h"

struct SDL_Renderer;
struct SDL_Surface;

class DukJavaScriptEngine : public JavaScriptEngine
{
  public:
    DukJavaScriptEngine();
    ~DukJavaScriptEngine() override;

    virtual void callEventListeners(std::string const & eventName, JSValue evt) override;
    virtual void callGlobalFunction(std::string const & function_name) override;
    virtual void callGlobalFunction(std::string const & function_name, JSValue parameters) override;
    virtual void evalFile(std::string const & filename) override;
    virtual void evalString(std::string const & str) override;
    virtual void initAudio() override;
    virtual void initBitmap(CanvasRenderingContext2D *) override;
    virtual void initCanvas(CanvasRenderingContext2D *) override;
    virtual void initNavigator(Navigator *) override;
    virtual void initWebsocket() override;
    virtual void preTick() override;

  private:
    using DukDestroyer = void (*)(duk_context *);
    std::unique_ptr<duk_context, DukDestroyer> ctx;

    SafeQueue<std::pair<std::function<void(duk_context *, MessageEvent)>, MessageEvent>>
        webSocketMessageQueue;
};
