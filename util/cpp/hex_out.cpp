#include <sstream>
#include <iomanip>

#include "..\h\hex_out.h"

string HexOut::m_HexOutString(string text)
{
	ostringstream utf8_str;
	for (unsigned char c : text)
	{
		utf8_str << std::hex << static_cast<int>(c) << ' ';
	}
	return utf8_str.str();
}
string HexOut::m_HexOutChar(const unsigned char *data, int len)
{
	ostringstream utf8_str;
	for (int i = 0; i < len; ++i)
	{
		utf8_str << std::hex << (int)data[i] << ' ';
	}
	return utf8_str.str();
}

