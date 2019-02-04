#pragma once

#include "../../duktape.h"

#include "../../../Navigator/Navigator.h"

namespace dcanvas {
	void initNavigator(duk_context * ctx, Navigator * navigator);
}
