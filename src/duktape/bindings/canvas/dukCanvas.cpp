#include "dukCanvas.h"

#include <string>

#include "../../../Logger/Logger.h"
#include "../../duktape.h"

#include "canvasFromCtx.h"
#include "drawImage.h"

static void defineProp(duk_context * ctx, std::string const & name, int (*getter)(duk_context *),
                       int (*setter)(duk_context *))
{
  duk_push_string(ctx, name.c_str());
  duk_push_c_function(ctx, getter, 0);
  duk_push_c_function(ctx, setter, 1);
  duk_def_prop(ctx, -4, DUK_DEFPROP_HAVE_GETTER | DUK_DEFPROP_HAVE_SETTER);
}

static void defineMethod(duk_context * ctx, std::string const & name, int (*method)(duk_context *),
                         int nargs)
{
  duk_push_string(ctx, name.c_str());
  duk_push_c_function(ctx, method, nargs);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);
}

static int fillRect(duk_context * ctx)
{
  auto state = dcanvas::canvasFromCtx(ctx);

  auto x = duk_require_int(ctx, -4);
  auto y = duk_require_int(ctx, -3);
  auto width = duk_require_int(ctx, -2);
  auto height = duk_require_int(ctx, -1);

  state->fillRect(x, y, width, height);

  return 0;
}

static int fillText(duk_context * ctx)
{
  auto state = dcanvas::canvasFromCtx(ctx);

  auto text = duk_require_string(ctx, -3);
  auto x = duk_require_int(ctx, -2);
  auto y = duk_require_int(ctx, -1);

  state->fillText(text, x, y);

  return 0;
}

static int getFillStyle(duk_context * ctx)
{
  auto state = dcanvas::canvasFromCtx(ctx);

  duk_push_string(ctx, state->getFillStyle().c_str());
  return 1;
}

static int setFillStyle(duk_context * ctx)
{
  auto state = dcanvas::canvasFromCtx(ctx);

  state->setFillStyle(std::string(duk_require_string(ctx, -1)));
  return 1;
}

static int getFont(duk_context * ctx)
{
  auto state = dcanvas::canvasFromCtx(ctx);

  duk_push_string(ctx, state->getFillStyle().c_str());
  return 1;
}

static int setFont(duk_context * ctx)
{
  auto state = dcanvas::canvasFromCtx(ctx);

  state->setFont(std::string(duk_require_string(ctx, -1)));
  return 1;
}

namespace dcanvas
{
auto logger = Logger::get();

void initCanvas(duk_context * ctx, CanvasRenderingContext2D * canvas)
{

  duk_get_global_string(ctx, "init");

  // Create an object with the property __internalPtr which is a pointer to the CanvasState
  // Apparently you can not define properties on a pointer as you could with a light userdata in
  // Lua?
  duk_push_object(ctx);
  duk_push_string(ctx, "\xFF"
                       "\xFF"
                       "internalPtr");
  duk_push_pointer(ctx, canvas);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

  defineProp(ctx, "fillStyle", getFillStyle, setFillStyle);
  defineProp(ctx, "font", getFont, setFont);

  defineMethod(ctx, "fillRect", fillRect, 4);
  defineMethod(ctx, "fillText", fillText, 3);
  defineMethod(ctx, "drawImage", dcanvas::drawImage, DUK_VARARGS);

  duk_pcall(ctx, 1);
  logger->info("init call finished");
}
}
