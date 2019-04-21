#pragma once

#include "../JSValue.h"
#include "duktape.h"

namespace dcanvas::dukUtils
{

void arraySplice(duk_context *, duk_idx_t arrayIdx, duk_idx_t startIdx, duk_idx_t deleteCount);

template <typename T>
T * fromCtx(duk_context * ctx)
{
    duk_push_this(ctx);
    duk_require_object(ctx, -1);
    duk_push_string(ctx, "\xFF"
                         "\xFF"
                         "internalPtr");
    duk_get_prop(ctx, -2);
    auto state = (T *)duk_get_pointer(ctx, -1);
    duk_pop_2(ctx);
    return state;
}

void pushToCtx(duk_context *, JSValue const &);
};
