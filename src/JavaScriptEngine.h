#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "Canvas/CanvasRenderingContext2D.h"
#include "JSValue.h"
#include "Navigator/Navigator.h"

struct SDL_Renderer;
struct SDL_Surface;

class JavaScriptEngine
{
  public:
    virtual void callEventListeners(std::string const & eventName, JSValue evt) = 0;
    virtual void callGlobalFunction(std::string const & functionName) = 0;
    virtual void callGlobalFunction(std::string const & functionName, JSValue parameters) = 0;
    // TODO: Maybe the above functions should return optional<JSValue>
    virtual JSValue callGlobalFunctionWithReturn(std::string const & functionName) = 0;
    virtual void evalFile(std::string const & filename) = 0;
    virtual void evalString(std::string const & str) = 0;
    // TODO: This is badly abstracted
    virtual void initAudio() = 0;
    virtual void initBitmap(CanvasRenderingContext2D *) = 0;
    virtual void initCanvas(CanvasRenderingContext2D *, SDL_Renderer *) = 0;
    virtual void initNavigator(Navigator *) = 0;
    virtual void initWebsocket() = 0;

    virtual void preTick() = 0;
    virtual void postTick() = 0;

    virtual ~JavaScriptEngine();
};
