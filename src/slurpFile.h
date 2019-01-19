#pragma once

#include <memory>
#include <string>

namespace dcanvas {
	std::string slurpFile(std::string const& filename);
	std::unique_ptr<void, void(*)(void*)> slurpFile(std::string const& filename, size_t * length);
}
