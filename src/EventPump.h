#pragma once

#include "JavaScriptEngine.h"

class EventPump {
public:
	virtual bool pumpEvents(IJavaScriptEngine * into) = 0;
	
	virtual ~EventPump();
};
