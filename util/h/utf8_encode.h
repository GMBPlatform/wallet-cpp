#include <iostream>
#include <ole2.h>
class UTF8Encode
{
public:
	std::string m_utf8_encode(const std::wstring &wstr);
	char *toUtf8(LPCSTR lpszText);
};


