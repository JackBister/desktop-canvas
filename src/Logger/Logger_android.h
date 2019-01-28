#pragma once

#include "Logger.h"

class AndroidLogger : public Logger {
public:
	virtual void info(char const * fmt, ...) override;
};
