#pragma once

#include "Logger.h"

class DesktopLogger : public Logger {
public:
	virtual void info(char const * fmt, ...) override;
};
