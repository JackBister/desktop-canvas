#pragma once

#include <functional>
#include <string>

namespace dcanvas {
	void watch_file(std::wstring filename, std::function<void()> onChange);
};
