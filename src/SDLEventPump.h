#pragma once

#include "EventPump.h"

class SDLEventPump : public EventPump {
	virtual void pumpEvents(IJavaScriptEngine * into) override;
};
