#include "Logger_android.h"

#include <android/log.h>

void AndroidLogger::info(char const * fmt, ...)
{
  va_list args;
  va_start(args, fmt);
  __android_log_vprint(ANDROID_LOG_INFO, "dcanvas", fmt, args);
  va_end(args);
}
