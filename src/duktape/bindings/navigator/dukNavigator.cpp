#include "dukNavigator.h"

#include "../../../Logger/Logger.h"
#include "../../dukUtils.h"

static auto logger = Logger::get();

static int getAxes(duk_context * ctx)
{
  auto gamepad = dcanvas::dukUtils::fromCtx<Gamepad>(ctx);
  auto axes = gamepad->getAxes();

  dcanvas::dukUtils::pushToCtx(ctx, std::vector<JSValue>(axes.cbegin(), axes.cend()));
  return 1;
}

static int getButtons(duk_context * ctx)
{
  auto gamepad = dcanvas::dukUtils::fromCtx<Gamepad>(ctx);
  auto buttons = gamepad->getButtons();
  std::vector<JSValue> buttonsAsJSValues;

  for (auto & button : buttons) {
    JSObject asJsObject = {{"pressed", button.getPressed()}, {"value", button.getValue()}};
    buttonsAsJSValues.push_back(asJsObject);
  }

  dcanvas::dukUtils::pushToCtx(ctx, buttonsAsJSValues);
  return 1;
}

static int getId(duk_context * ctx)
{
  auto gamepad = dcanvas::dukUtils::fromCtx<Gamepad>(ctx);
  auto id = gamepad->getId();

  dcanvas::dukUtils::pushToCtx(ctx, id);
  return 1;
}

static void pushGamepad(duk_context * ctx, Gamepad * gamepad)
{
  duk_push_object(ctx);
  duk_push_string(ctx, "\xFF"
                       "\xFF"
                       "internalPtr");
  duk_push_pointer(ctx, gamepad);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

  duk_push_string(ctx, "axes");
  duk_push_c_function(ctx, getAxes, 0);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

  duk_push_string(ctx, "buttons");
  duk_push_c_function(ctx, getButtons, 0);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);

  duk_push_string(ctx, "id");
  duk_push_c_function(ctx, getId, 0);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_GETTER);
}

static int getGamepads(duk_context * ctx)
{
  auto navigator = dcanvas::dukUtils::fromCtx<Navigator>(ctx);
  auto gamepads = navigator->getGamepads();
  auto arrIdx = duk_push_array(ctx);
  for (size_t i = 0; i < gamepads.size(); ++i) {
    if (gamepads[i] == nullptr) {
      duk_push_null(ctx);
    } else {
      pushGamepad(ctx, gamepads[i]);
    }
    duk_put_prop_index(ctx, arrIdx, i);
  }
  return 1;
}

void dcanvas::initNavigator(duk_context * ctx, Navigator * navigator)
{
  duk_push_object(ctx);
  duk_push_string(ctx, "\xFF"
                       "\xFF"
                       "internalPtr");
  duk_push_pointer(ctx, navigator);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

  duk_push_string(ctx, "getGamepads");
  duk_push_c_function(ctx, getGamepads, 0);
  duk_def_prop(ctx, -3, DUK_DEFPROP_HAVE_VALUE);

  duk_put_global_string(ctx, "navigator");
}
