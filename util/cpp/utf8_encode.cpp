#include <string>
#include <atlconv.h>
#include "..\wallet\util\h\utf8_encode.h"
std::string UTF8Encode::m_utf8_encode(const std::wstring &wstr)
{
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}
char *UTF8Encode::toUtf8(LPCSTR lpszText)
{
	int nByte;
	char *pszRet = NULL;

	nByte = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszText, -1, NULL, 0);
	LPWSTR wstr = new WCHAR[nByte];
	nByte = MultiByteToWideChar(CP_ACP, 0, (LPCSTR)lpszText, -1, wstr, nByte);

	nByte = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, 0, 0, 0, 0);
	pszRet = new char[nByte];
	nByte = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, pszRet, nByte, 0, 0);

	delete[] wstr;
	wstr = NULL;

	if (nByte == 0)
		return NULL;

	return pszRet; //결과값을 받은 함수에서 메모리 해제 해주어야 한다
}