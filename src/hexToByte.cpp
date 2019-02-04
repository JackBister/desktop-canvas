#include "hexToByte.h"

static uint8_t hexCharToByte(char hex)
{
    if (hex > 'A' && hex <= 'F') {
        return 10 + hex - 'A';
    }
    if (hex > 'a' && hex <= 'f') {
        return 10 + hex - 'a';
    }
    return hex - '0';
}

uint8_t dcanvas::hexToByte(std::string const & hexString)
{
    return hexCharToByte(hexString[0]) << 4 | hexCharToByte(hexString[1]);
}
