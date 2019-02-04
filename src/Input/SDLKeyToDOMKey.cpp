#include "SDLKeyToDOMKey.h"

#include <unordered_map>

static std::unordered_map<SDL_Keycode, std::string> KEYCODE_MAP{
    {SDLK_a, "KeyA"},
    {SDLK_b, "KeyB"},
    {SDLK_c, "KeyC"},
    {SDLK_d, "KeyD"},
    {SDLK_e, "KeyE"},
    {SDLK_f, "KeyF"},
    {SDLK_g, "KeyG"},
    {SDLK_h, "KeyH"},
    {SDLK_i, "KeyI"},
    {SDLK_j, "KeyJ"},
    {SDLK_k, "KeyK"},
    {SDLK_l, "KeyL"},
    {SDLK_m, "KeyM"},
    {SDLK_n, "KeyN"},
    {SDLK_o, "KeyO"},
    {SDLK_p, "KeyP"},
    {SDLK_q, "KeyQ"},
    {SDLK_r, "KeyR"},
    {SDLK_s, "KeyS"},
    {SDLK_t, "KeyT"},
    {SDLK_u, "KeyU"},
    {SDLK_v, "KeyV"},
    {SDLK_w, "KeyW"},
    {SDLK_x, "KeyX"},
    {SDLK_y, "KeyY"},
    {SDLK_z, "KeyZ"},

    {SDLK_0, "Digit0"},
    {SDLK_1, "Digit1"},
    {SDLK_2, "Digit2"},
    {SDLK_3, "Digit3"},
    {SDLK_4, "Digit4"},
    {SDLK_5, "Digit5"},
    {SDLK_6, "Digit6"},
    {SDLK_7, "Digit7"},
    {SDLK_8, "Digit8"},
    {SDLK_9, "Digit9"},

    {SDLK_RETURN, "Enter"},
    {SDLK_ESCAPE, "Escape"},
    {SDLK_BACKSPACE, "Backspace"},
    {SDLK_TAB, "Tab"},
    {SDLK_SPACE, "Space"},
    {SDLK_MINUS, "Minus"},
    {SDLK_EQUALS, "Equal"},

    {SDLK_LEFTBRACKET, "BracketLeft"},
    {SDLK_RIGHTBRACKET, "BracketRight"},
    {SDLK_BACKSLASH, "Backslash"},
    {SDLK_SEMICOLON, "Semicolon"},
    {SDLK_QUOTE, "Quote"},
    {SDLK_BACKQUOTE, "Backquote"},
    {SDLK_COMMA, "Comma"},
    {SDLK_PERIOD, "Period"},
    {SDLK_SLASH, "Slash"},
    {SDLK_CAPSLOCK, "CapsLock"},

    {SDLK_F1, "F1"},
    {SDLK_F2, "F2"},
    {SDLK_F3, "F3"},
    {SDLK_F4, "F4"},
    {SDLK_F5, "F5"},
    {SDLK_F6, "F6"},
    {SDLK_F7, "F7"},
    {SDLK_F8, "F8"},
    {SDLK_F9, "F9"},
    {SDLK_F10, "F10"},
    {SDLK_F11, "F11"},
    {SDLK_F12, "F12"},

    {SDLK_PRINTSCREEN, "PrintScreen"},
    {SDLK_SCROLLLOCK, "ScrollLock"},
    {SDLK_PAUSE, "Pause"},
    {SDLK_INSERT, "Insert"},
    {SDLK_HOME, "Home"},
    {SDLK_PAGEUP, "PageUp"},
    {SDLK_DELETE, "Delete"},
    {SDLK_END, "End"},
    {SDLK_PAGEDOWN, "PageDown"},

    {SDLK_RIGHT, "ArrowRight"},
    {SDLK_LEFT, "ArrowLeft"},
    {SDLK_DOWN, "ArrowDown"},
    {SDLK_UP, "ArrowUp"},

    // etc.
    // https://chromium.googlesource.com/external/mojo/+/dart_summit/ui/events/keycodes/dom4/keycode_converter_data.h

    {SDLK_LCTRL, "ControlLeft"},
    {SDLK_LSHIFT, "ShiftLeft"},
    {SDLK_LALT, "AltLeft"}};

std::optional<std::string> dcanvas::SDLKeyToDOMKey(SDL_Keycode keycode)
{
  if (KEYCODE_MAP.find(keycode) == KEYCODE_MAP.end()) {
    return {};
  }
  return KEYCODE_MAP[keycode];
}
