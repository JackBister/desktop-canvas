#pragma once

#include <stdarg.h>

class Logger
{
  public:
    static Logger * get();

    virtual void info(char const * fmt, ...) = 0;
};
