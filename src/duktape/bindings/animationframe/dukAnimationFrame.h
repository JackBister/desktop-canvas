#pragma once

#include "../../duktape.h"

namespace dcanvas
{
void executeAnimationFrameFunctions(duk_context * ctx);
void initAnimationFrame(duk_context * ctx);
};
