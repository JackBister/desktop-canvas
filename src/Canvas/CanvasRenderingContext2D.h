#pragma once

#include <string>

#include "SDLBitmap.h"
#include "TextMetrics.h"

class Canvas;

class CanvasRenderingContext2D
{
  public:
    // Though this method is not connected to the canvas context in JS, it is put here because it is
    // often coupled with how the bitmap is drawn
    virtual Bitmap * createBitmap(void * data, size_t size) = 0;

    virtual void drawImage(Bitmap * image, int dx, int dy) = 0;
    virtual void drawImage(Bitmap * image, int dx, int dy, int dWidth, int dHeight) = 0;
    virtual void drawImage(Bitmap * image, int sx, int sy, int sWidth, int sHeight, int dx, int dy,
                           int dWidth, int dHeight) = 0;

    virtual void fillRect(int x, int y, int width, int height) = 0;

    virtual void fillText(std::string const & text, int x, int y) = 0;
	virtual TextMetrics measureText(std::string const& text) = 0;

    virtual std::string const & getFont() = 0;
    virtual void setFont(std::string const & val) = 0;

	virtual std::string const & getTextAlign() = 0;
    virtual void setTextAlign(std::string const & val) = 0;

	virtual std::string const & getTextBaseline() = 0;
    virtual void setTextBaseline(std::string const & val) = 0;

    virtual std::string const & getFillStyle() = 0;
    virtual void setFillStyle(std::string const & val) = 0;

    virtual Canvas * getCanvas() = 0;

    virtual ~CanvasRenderingContext2D();
};
