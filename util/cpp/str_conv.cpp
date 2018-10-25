#include <locale>
#include <atlconv.h>
#include "..\wallet\util\h\str_conv.h"
std::wstring strconv::m_strconv(const std::string& _src)
{
	USES_CONVERSION;
	return std::wstring(A2W(_src.c_str()));
};

std::string strconv::m_strconv(const std::wstring& _src)
{
	USES_CONVERSION;
	return std::string(W2A(_src.c_str()));
};