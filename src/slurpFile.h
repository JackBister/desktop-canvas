#pragma once

#include <string>

namespace dcanvas {
	std::string slurpFile(std::string const& filename);
	void * slurpFile(std::string const& filename, size_t * length);
}
