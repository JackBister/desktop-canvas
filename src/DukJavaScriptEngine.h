#pragma once

#include "JavaScriptEngine.h"

#include "duktape/duktape.h"

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

private:
	using DukDestroyer = void(*)(duk_context *);
	std::unique_ptr<duk_context, DukDestroyer> ctx;
};
