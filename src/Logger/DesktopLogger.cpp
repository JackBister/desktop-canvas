#include "DesktopLogger.h"

#include <cstdio>
#include <stdarg.h>

void DesktopLogger::info(char const * fmt, ...) {
	va_list args;
	va_start(args, fmt);
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}
