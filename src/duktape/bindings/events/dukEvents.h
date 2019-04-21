#pragma once

#include <string>

#include "../../../JSValue.h"
#include "../../duktape.h"

namespace dcanvas
{
void initEvents(duk_context * ctx);
void callEventListeners(duk_context * ctx, std::string const & eventName, JSValue evt);
}