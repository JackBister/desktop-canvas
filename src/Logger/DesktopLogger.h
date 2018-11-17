#pragma once

#include "Logger.h"

class DesktopLogger : public ILogger {
public:
	virtual void info(char const * fmt, ...) override;
};
