#pragma once

#include <cstdint>
#include <string>

namespace dcanvas
{
uint8_t hexCharToByte(char hex);
uint8_t hexToByte(std::string const &);
}
