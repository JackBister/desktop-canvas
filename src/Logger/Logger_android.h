#pragma once

#include "Logger.h"

class AndroidLogger : public ILogger {
public:
	virtual void info(char const * fmt, ...) override;
};
