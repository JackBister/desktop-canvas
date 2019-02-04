#include "Logger.h"

#ifdef __ANDROID__
#include "Logger_android.h"
#else
#include "DesktopLogger.h"
#endif

Logger * Logger::get()
{
#ifdef __ANDROID__
    static auto ret = new AndroidLogger();
    return ret;
#else
    static auto ret = new DesktopLogger();
    return ret;
#endif
}
