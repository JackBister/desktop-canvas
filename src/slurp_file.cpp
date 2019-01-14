#include "slurp_file.h"

#include <SDL2/SDL.h>

#include "Logger/Logger.h"

static auto logger = ILogger::get();

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
