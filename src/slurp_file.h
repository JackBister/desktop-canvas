#pragma once

#include <string>

namespace dcanvas {
	std::string slurp_file(std::string const& filename);
	void * slurp_file(std::string const& filename, size_t * length);
}
