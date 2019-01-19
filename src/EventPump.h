#pragma once

#include "JavaScriptEngine.h"

class EventPump {
public:
	virtual void pumpEvents(IJavaScriptEngine * into) = 0;
	
	virtual ~EventPump();
};
