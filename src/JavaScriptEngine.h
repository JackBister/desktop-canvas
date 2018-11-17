#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "JSValue.h"

struct SDL_Renderer;
struct SDL_Surface;

class IJavaScriptEngine {
public:
	virtual void call_global_function(std::string const& function_name) = 0;
	virtual void call_global_function(std::string const& function_name, JSValue parameters) = 0;
	virtual void eval_file(std::string const& filename) = 0;
	virtual void eval_string(std::string const& str) = 0;
	// TODO: This is badly abstracted
	virtual void init_bitmap(SDL_Renderer *) = 0;
	virtual void init_canvas(SDL_Renderer *, SDL_Surface *) = 0;

	virtual ~IJavaScriptEngine() = 0;
};
