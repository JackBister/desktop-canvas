#pragma once

#include <stdarg.h>

class ILogger {
public:
	static ILogger * get();

	virtual void info(char const * fmt, ...) = 0;
};
