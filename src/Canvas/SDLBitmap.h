#pragma once

#include "Bitmap.h"

struct SDL_Texture;

class SDLBitmap : public Bitmap
{
public:
  SDLBitmap(SDL_Texture * texture, int height, int width);

  virtual int getHeight() override;
  virtual int getWidth() override;
  virtual void close() override;

  SDL_Texture * getTexture();

private:
  SDL_Texture * texture;
  int height;
  int width;
};
