#include "dukUtils.h"

#include "../Logger/Logger.h"

static auto logger = Logger::get();

void dcanvas::dukUtils::arraySplice(duk_context * ctx, duk_idx_t arrayIdx, duk_idx_t startIdx, duk_idx_t deleteCount)
{
	duk_dup(ctx, arrayIdx); // ..., []
	duk_get_prop_string(ctx, -1, "splice"); // ..., [], splice
	duk_swap_top(ctx, -2); // ..., splice, []
	duk_push_int(ctx, startIdx); // ..., splice, [], startIdx
	duk_push_int(ctx, deleteCount); // ..., splice, [], startIdx, deleteCount
	auto result = duk_pcall_method(ctx, 2); // ..., retval
	if (result == DUK_EXEC_ERROR) {
		duk_safe_to_string(ctx, -1);
		logger->info("arraySplice error %s", duk_require_string(ctx, -1));
	}
	duk_pop(ctx); // ...
}

void dcanvas::dukUtils::pushToCtx(duk_context * ctx, JSValue const & value)
{
    auto paramType = value.index();
    if (paramType == JSValue::Type::BOOL) {
        duk_push_boolean(ctx, std::get<bool>(value));
    } else if (paramType == JSValue::Type::DOUBLE) {
        duk_push_number(ctx, std::get<double>(value));
    } else if (paramType == JSValue::Type::OBJECT) {
        auto object = std::get<JSObject>(value);
        duk_push_bare_object(ctx);
        for (auto it = object.begin(); it != object.end(); ++it) {
            pushToCtx(ctx, it->second);
            duk_put_prop_string(ctx, -2, it->first.c_str());
        }
    } else if (paramType == JSValue::Type::STRING) {
        duk_push_string(ctx, std::get<std::string>(value).c_str());
    } else if (paramType == JSValue::Type::ARRAY) {
        auto arr = std::get<JSArray>(value);
        auto arrIdx = duk_push_array(ctx);
        for (size_t i = 0; i < arr.size(); ++i) {
            pushToCtx(ctx, arr[i]);
            duk_put_prop_index(ctx, arrIdx, i);
        }
    }
}
