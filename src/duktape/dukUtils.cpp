#include "dukUtils.h"

#include <stdexcept>

#include "../Logger/Logger.h"

static auto logger = Logger::get();

void dcanvas::dukUtils::arraySplice(duk_context * ctx, duk_idx_t arrayIdx, duk_idx_t startIdx,
                                    duk_idx_t deleteCount)
{
    duk_dup(ctx, arrayIdx);                 // ..., []
    duk_get_prop_string(ctx, -1, "splice"); // ..., [], splice
    duk_swap_top(ctx, -2);                  // ..., splice, []
    duk_push_int(ctx, startIdx);            // ..., splice, [], startIdx
    duk_push_int(ctx, deleteCount);         // ..., splice, [], startIdx, deleteCount
    auto result = duk_pcall_method(ctx, 2); // ..., retval
    if (result == DUK_EXEC_ERROR) {
        duk_safe_to_string(ctx, -1);
        logger->info("arraySplice error %s", duk_require_string(ctx, -1));
    }
    duk_pop(ctx); // ...
}

JSValue dcanvas::dukUtils::pullFromCtx(duk_context * ctx)
{
    if (duk_is_array(ctx, -1)) {
        auto len = duk_get_length(ctx, -1);
        std::vector<JSValue> arr;
        for (auto i = 0; i < len; ++i) {
            duk_get_prop_index(ctx, -1, i);
            arr.push_back(pullFromCtx(ctx));
            duk_pop(ctx);
        }
        return arr;
    } else if (duk_is_boolean(ctx, -1)) {
        return (bool)duk_get_boolean(ctx, -1);
    } else if (duk_is_null(ctx, -1)) {
        return nullptr;
    } else if (duk_is_number(ctx, -1)) {
        return duk_get_number(ctx, -1);
    } else if (duk_is_object(ctx, -1)) {
        duk_enum(ctx, -1, 0);
        JSObject obj;
        while (duk_next(ctx, -1, 1)) {
            auto key = duk_to_string(ctx, -2);
            auto value = pullFromCtx(ctx);
            obj.push_back(std::make_pair(key, value));
            duk_pop_2(ctx);
        }
        duk_pop(ctx);
        return obj;
    } else if (duk_is_string(ctx, -1)) {
        auto str = duk_get_string(ctx, -1);
        return std::string(str);
    } else {
        logger->info("Unhandled JSValue type at top of stack %d", duk_get_type(ctx, -1));
        throw std::runtime_error("Unhandled JSValue type at top of stack");
    }
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
    } else if (paramType == JSValue::Type::NULLPTR) {
        duk_push_null(ctx);
	}
}
