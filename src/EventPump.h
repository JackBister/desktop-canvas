#pragma once

#include "JavaScriptEngine.h"

class EventPump
{
  public:
    virtual bool pumpEvents(JavaScriptEngine * into) = 0;

    virtual ~EventPump();
};
