#pragma once

class CanvasRenderingContext2D;

class Canvas
{
  public:
    virtual int getHeight() = 0;
    virtual void setHeight(int height) = 0;

    virtual int getWidth() = 0;
    virtual void setWidth(int width) = 0;

    virtual CanvasRenderingContext2D * getContext() = 0;
};
