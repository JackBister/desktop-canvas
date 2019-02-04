#pragma once

class Bitmap
{
  public:
    virtual int getHeight() = 0;
    virtual int getWidth() = 0;

    virtual void close() = 0;
};
