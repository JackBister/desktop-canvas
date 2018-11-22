#include "slurp_file.h"

// Workaround for NDK retardation
#ifdef __ANDROID__
#include "SDL.h"
#else
#include <SDL2/SDL.h>
#endif

#include "Logger/Logger.h"

auto logger = ILogger::get();

std::string dcanvas::slurp_file(std::string const& filename)
{
	size_t length = 0;
	char * buffer = (char *) SDL_LoadFile(filename.c_str(), &length);

	auto ret = std::string(buffer, buffer + length);
	free(buffer);
	return ret;
}

void * dcanvas::slurp_file(std::string const& filename, size_t * length) {
	return SDL_LoadFile(filename.c_str(), length);
}
