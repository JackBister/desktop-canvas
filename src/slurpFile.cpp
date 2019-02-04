#include "slurpFile.h"

#include <SDL2/SDL.h>

#include "Logger/Logger.h"

static auto logger = Logger::get();

std::string dcanvas::slurpFile(std::string const & filename)
{
  size_t length = 0;
  char * buffer = (char *)SDL_LoadFile(filename.c_str(), &length);

  auto ret = std::string(buffer, buffer + length);
  SDL_free(buffer);
  return ret;
}

std::unique_ptr<void, void (*)(void *)> dcanvas::slurpFile(std::string const & filename,
                                                           size_t * length)
{
  return std::unique_ptr<void, void (*)(void *)>(SDL_LoadFile(filename.c_str(), length), SDL_free);
}
