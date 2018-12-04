#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "JSValue.h"
#include "Canvas/CanvasRenderingContext2D.h"

struct SDL_Renderer;
struct SDL_Surface;

class IJavaScriptEngine {
public:
	virtual void call_global_function(std::string const& function_name) = 0;
	virtual void call_global_function(std::string const& function_name, JSValue parameters) = 0;
	virtual void eval_file(std::string const& filename) = 0;
	virtual void eval_string(std::string const& str) = 0;
	// TODO: This is badly abstracted
	virtual void init_bitmap(ICanvasRenderingContext2D *) = 0;
	virtual void init_canvas(ICanvasRenderingContext2D *) = 0;
	virtual void init_websocket() = 0;

	virtual void pre_tick() = 0;

	virtual ~IJavaScriptEngine();
};
