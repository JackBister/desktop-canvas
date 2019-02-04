#pragma once

#include <memory>
#include <variant>
#include <vector>

class JSValue;

using JSArray = std::vector<JSValue>;
using JSObject = std::vector<std::pair<std::string, JSValue>>;

class JSValue : public std::variant<bool, double, std::string, JSObject, JSArray>
{
public:
  enum Type { BOOL = 0, DOUBLE = 1, STRING = 2, OBJECT = 3, ARRAY = 4 };

  using std::variant<bool, double, std::string, JSObject, JSArray>::variant;
};
