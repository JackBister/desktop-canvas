#include "hex_to_byte.h"


static uint8_t hex_char_to_byte(char hex) {
	if (hex > 'A' && hex <= 'F') {
		return 10 + hex - 'A';
	}
	if (hex > 'a' && hex <= 'f') {
		return 10 + hex - 'a';
	}
	return hex - '0';
}

uint8_t dcanvas::hex_to_byte(std::string const& hexString)
{
	return hex_char_to_byte(hexString[0]) << 4 | hex_char_to_byte(hexString[1]);
}
