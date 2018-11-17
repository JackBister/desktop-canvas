#include "DukJavaScriptEngine.h"

#include "duktape/bindings/bitmap/duk_bitmap.h"
#include "duktape/bindings/canvas/duk_canvas.h"
#include "duktape/bindings/console/duk_console.h"

#include "slurp_file.h"

static void push_js_value(duk_context * ctx, JSValue value) {
	auto paramType = value.index();
	if (paramType == JSValue::Type::BOOL) {
		duk_push_boolean(ctx, std::get<bool>(value));
	} else if (paramType == JSValue::Type::DOUBLE) {
		duk_push_number(ctx, std::get<double>(value));
	} else if (paramType == JSValue::Type::OBJECT) {
		auto object = std::get<JSObject>(value);
		duk_push_bare_object(ctx);
		for (auto it = object.begin(); it != object.end(); ++it) {
			push_js_value(ctx, it->second);
			duk_put_prop_string(ctx, -2, it->first.c_str());
		}
	} else if (paramType == JSValue::Type::STRING) {
		duk_push_string(ctx, std::get<std::string>(value).c_str());
	}
}

DukJavaScriptEngine::DukJavaScriptEngine() : ctx(duk_create_heap_default(), duk_destroy_heap)
{
	duk_console_init(ctx.get(), DUK_CONSOLE_PROXY_WRAPPER);
}

DukJavaScriptEngine::~DukJavaScriptEngine()
{
}

void DukJavaScriptEngine::call_global_function(std::string const& function_name)
{
	duk_get_global_string(ctx.get(), function_name.c_str());
	duk_pcall(ctx.get(), 0);
	duk_pop(ctx.get());
}

void DukJavaScriptEngine::call_global_function(std::string const& function_name, JSValue parameters)
{
	duk_get_global_string(ctx.get(), function_name.c_str());

	push_js_value(ctx.get(), parameters);

	duk_pcall(ctx.get(), 1);
	duk_pop(ctx.get());
}

void DukJavaScriptEngine::eval_file(std::string const& filename)
{
	auto fileContents = dcanvas::slurp_file(filename);
	duk_eval_string(ctx.get(), fileContents.c_str());
}

void DukJavaScriptEngine::eval_string(std::string const& str)
{
	duk_eval_string(ctx.get(), str.c_str());
}

void DukJavaScriptEngine::init_bitmap(SDL_Renderer * renderer)
{
	dcanvas::init_bitmap(ctx.get(), renderer);
}

void DukJavaScriptEngine::init_canvas(SDL_Renderer * renderer, SDL_Surface * surface)
{
	dcanvas::init_canvas(ctx.get(), renderer, surface);
}
