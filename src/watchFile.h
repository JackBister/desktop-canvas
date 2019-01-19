#pragma once

#include <functional>
#include <string>

namespace dcanvas {
	void watchFile(std::wstring filename, std::function<void()> onChange);
};
