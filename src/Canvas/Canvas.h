#pragma once

class CanvasRenderingContext2D;

class Canvas
{
  public:
    virtual int getHeight() = 0;
    virtual int getWidth() = 0;

    virtual CanvasRenderingContext2D * getContext() = 0;
};
