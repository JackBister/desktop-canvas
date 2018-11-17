#pragma once

#include <memory>
#include <unordered_map>
#include <variant>

class JSValue;

using JSObject = std::unordered_map<std::string, JSValue>;

class JSValue : public std::variant<bool, double, std::string, JSObject> {
public:
	enum Type {
		BOOL = 0,
		DOUBLE = 1,
		STRING = 2,
		OBJECT = 3
	};

	using std::variant<bool, double, std::string, JSObject>::variant;
};
