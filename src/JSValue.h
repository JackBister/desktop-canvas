#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class JSValue;

using JSArray = std::vector<JSValue>;
using JSObject = std::vector<std::pair<std::string, JSValue>>;

class JSValue : public std::variant<bool, double, std::string, JSObject, JSArray, nullptr_t>
{
  public:
    enum Type { BOOL = 0, DOUBLE = 1, STRING = 2, OBJECT = 3, ARRAY = 4, NULLPTR = 5 };

    using std::variant<bool, double, std::string, JSObject, JSArray, nullptr_t>::variant;
};
