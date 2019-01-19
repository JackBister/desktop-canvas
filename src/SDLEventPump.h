#pragma once

#include "EventPump.h"

class SDLEventPump : public EventPump {
	virtual bool pumpEvents(IJavaScriptEngine * into) override;
};
